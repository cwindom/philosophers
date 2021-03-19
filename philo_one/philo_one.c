#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <stdio.h>
#include <pthread.h>

int		g_alive;

typedef struct			s_data
{
	time_t			time_to_eat;
	time_t			time_to_sleep;
	time_t			time_to_die;

	time_t start;
	unsigned int		times_to_eat;
	unsigned int		num_of_phil;
	pthread_mutex_t		*forks;
	pthread_mutex_t		write_lock;
	pthread_mutex_t		death_lock;
}						t_data;

typedef struct			s_phil
{
	int					id;
	int					l_fork;
	int					r_fork;
	int					full;
	unsigned int		has_eaten;
	time_t			last_meal;
	int					eating;
	pthread_t			*tid;
	t_data				*data;
}						t_phil;

time_t mygettimeofday()
{
	struct timeval	time;
	time_t			t;

	gettimeofday(&time, NULL);
	t = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (t);
}
int			ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}
void error(char *err)
{
	printf("Error: %s\n", err);
	//write(1, "Error: ", 7);
	//write(1, err, ft_strlen(err));
	//write(1, "\n", 1);
	exit(1);
}
int	atoi_philo(const char *str)
{
	int res;
	int i;

	i = 0;
	res = 0;
	while ((str[i] >= '0') && (str[i] <= '9'))
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	if (str[i] && (str[i] < '0' || str[i] > '9'))
		error("please enter correct arguments");
	return (res);
}

void	write_message(t_phil *phil, char *str)
{
	pthread_mutex_lock(&phil->data->write_lock);
	printf("%ld %d %s", mygettimeofday() - phil->data->start, phil->id + 1, str);
	//ft_putnbr_fd(mygettimeofday() - phil->data->start, 1);
	write(1, " ", 1);
	//ft_putnbr_fd(phil->id + 1, 1);
	//write(1, str, ft_strlen(str));
	pthread_mutex_unlock(&phil->data->write_lock);
}
int		write_error(char *str)
{
	write(1, str, ft_strlen(str));
	return (1);
}

static int	check_for_death(t_phil *phil, t_data *data, int i)
{
	time_t	now;

	now = mygettimeofday();
	if ((!phil[i].eating) &&
		(now > (phil[i].last_meal + data->time_to_die)))
	{
		pthread_mutex_lock(&phil->data->write_lock);
		g_alive = 0;
		printf("%ld %d died\n", mygettimeofday() - phil->data->start, phil->id + 1);
		//ft_putnbr_fd(mygettimeofday() - phil->data->start, 1, );
		//write(1, " ", 1);
		//ft_putnbr_fd(phil->id + 1, 1);
		//write(1, " died\n", 6);
		return (1);
	}
	return (0);
}

static int	check_if_eatten_enough(t_phil *phil, unsigned int count)
{
	if (count == phil->data->num_of_phil)
	{
		pthread_mutex_lock(&phil->data->write_lock);
		write(1, "Philosophers has eaten enough\n", 30);
		return (1);
	}
	return (0);
}

void		*check_state(void *arg)
{
	t_phil 			*phil;
	unsigned int	i;
	unsigned int	count;

	phil = (t_phil *)arg;
	while (1)
	{
		i = 0;
		count = 0;
		while (i < phil->data->num_of_phil)
		{
			if (check_for_death(phil, phil->data, i))
				return ((void*)0);
			if (phil->data->times_to_eat)
			{
				if (phil[i].full)
					count++;
				if (check_if_eatten_enough(phil, count))
					return ((void*)0);
			}
			i++;
		}
	}
}
static void	count_meals(t_phil *phil)
{
	if (phil->data->times_to_eat)
	{
		phil->has_eaten++;
		if (phil->data->times_to_eat == phil->has_eaten)
			phil->full = 1;
	}
}
static void	eating(t_phil *phil)
{
	time_t start;
	time_t finish;

	pthread_mutex_lock(&phil->data->forks[phil->l_fork]);
	write_message(phil, " has taken a fork\n");
	pthread_mutex_lock(&phil->data->forks[phil->r_fork]);
	write_message(phil, " has taken a fork\n");
	phil->eating = 1;
	write_message(phil, " is eating\n");
	start = mygettimeofday();
	finish = start;
	phil->last_meal = mygettimeofday();
	while ((finish - start) < phil->data->time_to_eat)
	{
		usleep(1);
		finish = mygettimeofday();
	}
	count_meals(phil);
	phil->eating = 0;
	g_alive ? pthread_mutex_unlock(&phil->data->forks[phil->l_fork]) : 0;
	g_alive ? pthread_mutex_unlock(&phil->data->forks[phil->r_fork]) : 0;
}
static void	sleeping(t_phil *phil)
{
	write_message(phil, " is sleeping\n");
	usleep(phil->data->time_to_sleep * 1000);
}
static void	*action(void *arg)
{
	t_phil		*phil;

	phil = (t_phil *)arg;
	phil->last_meal = mygettimeofday();
	while (g_alive)
	{
		g_alive ? eating(phil) : 0;
		g_alive ? sleeping(phil) : 0;
		g_alive ? write_message(phil, " is thinking\n") : 0;
	}
	return ((void*)0);
}
void		*start_threads(t_data *data, t_phil *phil)
{
	unsigned int	i;
	pthread_t		tid;

	i = 0;
	data->start = mygettimeofday();
	while (i < data->num_of_phil)
	{
		pthread_create(phil[i].tid, NULL, action, (void *)&phil[i]);
		pthread_detach(*phil[i].tid);
		usleep(100);
		i++;
	}
	pthread_create(&tid, NULL, &check_state, (void *)phil);
	pthread_join(tid, NULL);
	usleep(100000);
	return ((void*)0);
}
//void		free_all(t_all *all)
//{
//	unsigned int i;
//
//	i = 0;
//	if (all->data->forks)
//	{
//		while (i < all->data->num_of_phil)
//		{
//			pthread_mutex_destroy(&all->data->forks[i]);
//			i++;
//		}
//	}
//	pthread_mutex_destroy(&all->data->write_lock);
//	free(all->data->forks);
//	if (all->phil)
//		free(all->phil);
//	all->phil = NULL;
//}
t_phil 	*init_phil(t_phil *phi, t_data *data)
{
	int i = 0;
	t_phil *phil;
	phil = malloc(sizeof(t_phil) * data->num_of_phil);
	while (i < data->num_of_phil)
	{
		phil[i].data = data;
		phil[i].id = i;
		phil[i].l_fork = i;
		phil[i].r_fork = (i + 1) % data->num_of_phil;
		phil[i].last_meal = 0;
		phil[i].eating = 0;
		phil[i].has_eaten = 0;
		phil[i].full = 0;
		phil[i].tid = malloc(sizeof(pthread_t) * 100);
		i++;
	}

	return phil;
}
void		init_vars(char **argv, int argc, t_data *data, t_phil *phil)
{
	unsigned int i;

	i = 0;
	g_alive = 1;
	pthread_mutex_init(&data->write_lock, NULL);
	pthread_mutex_init(&data->death_lock, NULL);
	data->num_of_phil = atoi_philo(argv[1]);
	data->time_to_die = atoi_philo(argv[2]);
	data->time_to_eat = atoi_philo(argv[3]);
	data->time_to_sleep = atoi_philo(argv[4]);
	if (argc == 6)
		data->times_to_eat = atoi_philo(argv[5]);
	else
		data->times_to_eat = 0;
	//phil = (t_phil *)malloc(sizeof(t_phil ) * data->num_of_phil);
	data->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) *
												 data->num_of_phil);
	while (i < data->num_of_phil)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	printf("%d, %llu, %llu, %llu", data->num_of_phil, data->time_to_die,
		   data->time_to_eat, data->time_to_sleep);
}
int			main(int argc, char **argv)
{
	t_data			data;
	t_phil			*phil;

	if (argc < 5 || argc > 6)
		return (write_error("wrong number of arguments\n"));
	init_vars(argv, argc, &data, phil);
	if (data.num_of_phil > 200 || data.time_to_die < 60 || data.time_to_sleep< 60)
		return (0);

		phil = init_phil(phil, &data);
	start_threads(&data, phil);
	//free_all(&all);
	return (0);
}

