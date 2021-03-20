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
	pthread_mutex_t	print;
}					t_data;
typedef struct			s_phil
{
	int i;
	int					id;
	int					l_fork;
	int					r_fork;
	int					full;
	unsigned int		has_eaten;
	time_t				last_meal;
	int					eating;
	pthread_t			*threads;
	//pthread_mutex_t	print;
	t_data				*data;

}						t_phil;


time_t mygettimeofday()//нет вопросов)
{
	struct timeval	time;
	time_t			t;

	gettimeofday(&time, NULL);
	t = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (t);
}
void error(char *err, int exitcode)//нет вопросов)
{
	printf("Error: %s\n", err);
	exit(exitcode);
}
int	atoi_philo(const char *str)//нет вопросов)
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
		error("please enter correct arguments", 1);
	return (res);
}

void		free_all(t_phil *p, t_data *d)
{
	int i;

	i = 0;
	if (d->forks)
	{
		while (i < d->num)
		{
			pthread_mutex_destroy(&d->forks[i]);
			i++;
		}
	}
	pthread_mutex_destroy(&d->print);
	//pthread_mutex_destroy(&phil->print);
	free(d->forks);
	if (p)
		free(p);
	p = NULL;
}
void	write_message(t_phil *p, char *str)
{
	pthread_mutex_lock(&p->data->print);
	//pthread_mutex_lock(&phil->print);
	printf("%ld %d %s", mygettimeofday() - p->data->start, p->id + 1, str);
	pthread_mutex_unlock(&p->data->print);
	//pthread_mutex_unlock(&phil->print);
}
static int	check_for_death(t_phil *p, t_data *d, int i)
{
	time_t	now;

	now = mygettimeofday();
	if ((!p[i].eating) && (now > (p[i].last_meal + d->time_to_die)))
	{
		pthread_mutex_lock(&p->data->print);
		//pthread_mutex_lock(&phil->print);
		g_alive = 0;
		printf("%ld %d died\n", mygettimeofday() - p->data->start, p->id + 1);
		return (1);
	}
	return (0);
}
static int	check_if_eatten_enough(t_phil *p, unsigned int count)
{
	if (count == p->data->num)
	{
		pthread_mutex_lock(&p->data->print);
		//pthread_mutex_lock(&phil->print);
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
static void	count_meals(t_phil *p)
{
	if (p->data->num_eat)
	{
		p->has_eaten++;
		if (p->data->num_eat == p->has_eaten)
			p->full = 1;
	}
}
static void	eating(t_phil *p)
{
	time_t start;
	time_t finish;

	pthread_mutex_lock(&p->data->forks[p->l_fork]);
	write_message(p, " has taken a fork\n");
	pthread_mutex_lock(&p->data->forks[p->r_fork]);
	write_message(p, " has taken a fork\n");
	p->eating = 1;
	write_message(p, " is eating\n");
	start = mygettimeofday();
	finish = start;
	p->last_meal = mygettimeofday();
	while ((finish - start) < p->data->time_to_eat)
	{
		usleep(1);
		finish = mygettimeofday();
	}
	count_meals(p);
	p->eating = 0;
	g_alive ? pthread_mutex_unlock(&p->data->forks[p->l_fork]) : 0;
	g_alive ? pthread_mutex_unlock(&p->data->forks[p->r_fork]) : 0;
}
static void	sleeping(t_phil *p)
{
	write_message(p, " is sleeping\n");
	usleep(p->data->time_to_sleep * 1000);
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


void		*start_threads(t_data *d, t_phil *p)
{
	//pthread_t		tid;

	p->i = 0;
	d->start = mygettimeofday();
	while (p->i < d->num)
	{
		pthread_create(p[p->i].threads, NULL, action, (void *)&p[p->i]);
		pthread_detach(*p[p->i].threads);
		usleep(100);
		p->i++;
	}
//	pthread_create(&tid, NULL, &check_state, (void *)p);
//	pthread_join(tid, NULL);
	pthread_create(p->threads, NULL, &check_state, (void *)p);
	pthread_join(*p->threads, NULL);
	usleep(100000);
	return (NULL);
}


t_phil 	*init(t_data *d) //вопрос к полям структуры
{
	t_phil *p;

	p = malloc(sizeof(t_phil) * d->num);
	p->i = 0;
	while (p->i < d->num)
	{
		p[p->i].data = d;
		p[p->i].id = p->i;
		p[p->i].l_fork = p->i;
		p[p->i].r_fork = (p->i + 1) % d->num;
		p[p->i].last_meal = 0;
		p[p->i].eating = 0;
		p[p->i].has_eaten = 0;
		p[p->i].full = 0;
		p[p->i].threads = malloc(sizeof(pthread_t));
		p->i++;
	}
	pthread_mutex_init(&p->data->print, NULL);
	d->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * d->num);
	p->i = -1;
	while (++p->i < d->num)
		pthread_mutex_init(&d->forks[p->i], NULL);
	return p;
}
void		parse_argv(char **av, t_data *d)//вопрос к d->num_eat = 0;
{
	g_alive = 1;
	d->num = atoi_philo(av[1]);
	d->time_to_die = atoi_philo(av[2]);
	d->time_to_eat = atoi_philo(av[3]);
	d->time_to_sleep = atoi_philo(av[4]);
	if (av[5])
		d->num_eat = atoi_philo(av[5]);
	else
		d->num_eat = 0;
	if (d->num > 200 || d->time_to_die < 60 || d->time_to_sleep < 60)
		error("wrong arguments", 1);
	//phil = (t_phil *)malloc(sizeof(t_phil ) * data->num_of_phil);
}
int			main(int ac, char **av) //нет вопросов)
{
	t_data d;
	t_phil *p;

	if (ac == 5 || ac == 6)
	{
		p = NULL;
		parse_argv(av, &d);
		p = init(&d);
		start_threads(&d, p);
		free_all(p, &d);
	}
	else
		error("wrong arguments", 1);
	return (0);
}

