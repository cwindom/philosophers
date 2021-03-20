#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <pthread.h>

int		g_alive;

typedef struct			s_data
{
	int				num;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_eat;
	long			start;
	pthread_mutex_t	*forks;
	//pthread_mutex_t	print;
}					t_data;
typedef struct			s_phil
{
	int					id;
	int					l_fork;
	int					r_fork;
	int					full;
	unsigned int		has_eaten;
	time_t				last_meal;
	int					eating;
	pthread_t			*threads;
	pthread_mutex_t	print;
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
void error(char *err)
{
	printf("Error: %s\n", err);
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
	//pthread_mutex_lock(&phil->data->print);
	pthread_mutex_lock(&phil->print);
	printf("%ld %d %s", mygettimeofday() - phil->data->start, phil->id + 1, str);
	//pthread_mutex_unlock(&phil->data->print);
	pthread_mutex_unlock(&phil->print);
}
static int	check_for_death(t_phil *phil, t_data *data, int i)
{
	time_t	now;

	now = mygettimeofday();
	if ((!phil[i].eating) &&
		(now > (phil[i].last_meal + data->time_to_die)))
	{
		//pthread_mutex_lock(&phil->data->print);
		pthread_mutex_lock(&phil->print);
		g_alive = 0;
		printf("%ld %d died\n", mygettimeofday() - phil->data->start, phil->id + 1);
		return (1);
	}
	return (0);
}
static int	check_if_eatten_enough(t_phil *phil, unsigned int count)
{
	if (count == phil->data->num)
	{
		//pthread_mutex_lock(&phil->data->print);
		pthread_mutex_lock(&phil->print);
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
		while (i < phil->data->num)
		{
			if (check_for_death(phil, phil->data, i))
				return (NULL);
			if (phil->data->num_eat)
			{
				if (phil[i].full)
					count++;
				if (check_if_eatten_enough(phil, count))
					return (NULL);
			}
			i++;
		}
	}
}

static void	count_meals(t_phil *phil)
{
	if (phil->data->num_eat)
	{
		phil->has_eaten++;
		if (phil->data->num_eat == phil->has_eaten)
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
	return (NULL);
}

void		*start_threads(t_data *data, t_phil *phil)
{
	unsigned int	i;
	pthread_t		tid;

	i = 0;
	data->start = mygettimeofday();
	while (i < data->num)
	{
		pthread_create(phil[i].threads, NULL, action, (void *)&phil[i]);
		pthread_detach(*phil[i].threads);
		usleep(100);
		i++;
	}
	pthread_create(&tid, NULL, &check_state, (void *)phil);
	pthread_join(tid, NULL);
	usleep(100000);
	return (NULL);
}
void		free_all(t_phil *phil, t_data *data)
{
	unsigned int i;

	i = 0;
	if (data->forks)
	{
		while (i < data->num)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
	}
	//pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&phil->print);
	free(data->forks);
	if (phil)
		free(phil);
	phil = NULL;
}
t_phil 	*init_phil(t_data *d) //пофиксить поля структуры
{
	int i;
	t_phil *p;

	i = 0;
	p = malloc(sizeof(t_phil) * d->num);
	while (i < d->num)
	{
		p[i].data = d;
		p[i].id = i;
		p[i].l_fork = i;
		p[i].r_fork = (i + 1) % d->num;
		p[i].last_meal = 0;
		p[i].eating = 0;
		p[i].has_eaten = 0;
		p[i].full = 0;
		p[i].threads = malloc(sizeof(pthread_t));
		i++;
	}
	pthread_mutex_init(&p->print, NULL);
	return p;
}
void		parse_argv(char **av, t_data *d)
{
	unsigned int i;

	i = 0;
	g_alive = 1;
	//pthread_mutex_init(&d->print, NULL);
	d->num = atoi_philo(av[1]);
	d->time_to_die = atoi_philo(av[2]);
	d->time_to_eat = atoi_philo(av[3]);
	d->time_to_sleep = atoi_philo(av[4]);
	if (av[5])
		d->num_eat = atoi_philo(av[5]);
	else
		d->num_eat = 0;
	if (d->num > 200 || d->time_to_die < 60 || d->time_to_sleep < 60)
		error("wrong arguments");
	//phil = (t_phil *)malloc(sizeof(t_phil ) * data->num_of_phil);
	d->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * d->num);
	while (i < d->num)
	{
		pthread_mutex_init(&d->forks[i], NULL);
		i++;
	}
}

int			main(int ac, char **av)
{
	t_data d;
	t_phil *p;

	if (ac == 5 || ac == 6)
	{
		p = NULL;
		//pthread_mutex_init(&p->print, NULL);
		parse_argv(av, &d);
		p = init_phil(&d);
		start_threads(&d, p);
		//free_all(&all);
	}
	else
		error("wrong arguments");
	return (0);
}

