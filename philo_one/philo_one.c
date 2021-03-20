#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <pthread.h>

typedef struct			s_data
{
	int				num;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_eat;
	long			time_start;
	pthread_mutex_t	print;
	pthread_mutex_t	*forks;
	//int				death;
}					t_data;

typedef struct	s_phil
{
	int			i;
	int			id;
	int			left;
	int			right;

	int			eat_up;
	long		last_meal;
	int			is_eating;
	pthread_t	*threads;
	t_data		*data;
}						t_phil;

long mygettimeofday()
{
	struct timeval	time;
	long			t;

	gettimeofday(&time, NULL);
	t = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (t);
}
void error(char *err, int exitcode)
{
	printf("Error: %s\n", err);
	exit(exitcode);
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
		error("please enter correct arguments", 1);
	return (res);
}

static void	eating(t_phil *p)
{
	long start;
	long finish;

	pthread_mutex_lock(&p->data->forks[p->left]);
	pthread_mutex_lock(&p->data->print);
	printf("%ld %d has taken a fork\n", mygettimeofday() - p->data->time_start, p->id + 1);
	pthread_mutex_unlock(&p->data->print);
	pthread_mutex_lock(&p->data->forks[p->right]);
	pthread_mutex_lock(&p->data->print);
	printf("%ld %d has taken a fork\n", mygettimeofday() - p->data->time_start, p->id + 1);
	pthread_mutex_unlock(&p->data->print);
	p->is_eating = 1;
	pthread_mutex_lock(&p->data->print);
	printf("%ld %d is eating\n", mygettimeofday() - p->data->time_start, p->id + 1);
	pthread_mutex_unlock(&p->data->print);
	start = mygettimeofday();
	finish = start;
	p->last_meal = mygettimeofday();
	while ((finish - start) < p->data->time_to_eat)
	{
		usleep(10);
		finish = mygettimeofday();
	}
	if (p->data->num_eat != -1)
		p->eat_up++;

	//здесь скользко, нужна задержка, чтобы не пытаться положить вилки, если
	// мы умерли
	usleep(10);
	pthread_mutex_unlock(&p->data->forks[p->right]);
	pthread_mutex_unlock(&p->data->forks[p->left]);
	p->is_eating = 0;
}
static void	sleeping(t_phil *p)
{
	pthread_mutex_lock(&p->data->print);
	printf("%ld %d is sleeping\n", mygettimeofday() - p->data->time_start,p->id + 1);
	pthread_mutex_unlock(&p->data->print);
	usleep(p->data->time_to_sleep * 1000);
}

static void	*lifetime(void *arg)
{
	t_phil		*p;

	p = (t_phil *)arg;
	p->last_meal = mygettimeofday();
	while (1)
	{
		eating(p);
		sleeping(p);
		pthread_mutex_lock(&p->data->print);
		printf("%ld %d is thinking\n", mygettimeofday() -p->data->time_start, p->id + 1);
		pthread_mutex_unlock(&p->data->print);
	}
	return (NULL);
}

void		*check_philo(void *arg)
{
	t_phil *p;
	int	count;

	p = (t_phil *)arg;
	while (1)
	{
		p->i = -1;
		count = 0;
		while (++p->i < p->data->num)
		{
//			time_t	now;
//			now = mygettimeofday();
//			if ((!p[p->i].is_eating) && (now > (p[p->i].last_meal+p->data->time_to_die)))
//			{
//				printf("is eating: %d", p[p->i].is_eating);
//				pthread_mutex_lock(&p->data->print);
//				p->data->death = 1;
//				printf("test: %ld\n", p->last_meal - p->data->time_start);
//				printf("%ld %d died\n", mygettimeofday() - p->data->time_start, p->id + 1);
//				return (NULL);
//			}
			if (p->data->num_eat != -1)
			{
				if (p[p->i].eat_up == p->data->num_eat)
					count++;
				if (count == p->data->num)
				{
					pthread_mutex_lock(&p->data->print);
					printf("All philosophers ate %d times\n",p->data->num_eat);
					return (NULL);
				}
			}
			usleep(100);
		}
		usleep(100);
	}
}
void		*start_threads(t_data *d, t_phil *p)
{
	d->time_start = mygettimeofday();
	long	now;
	p->i = -1;
	while (++p->i < d->num)
	{
		pthread_create(p[p->i].threads, NULL, &lifetime, (void *)&p[p->i]);
		usleep(200);
		pthread_detach(*p[p->i].threads);
	}
	while(1)
	{
		p->i = -1;
		int count = 0;
		while (++p->i < p->data->num)
		{
			now = mygettimeofday();
			if ((!p[p->i].is_eating) && (now > (p[p->i].last_meal+p->data->time_to_die)))
			{
				pthread_mutex_lock(&p->data->print);
				printf("%ld %d died\n", mygettimeofday() -
				p->data->time_start, p[p->i].id + 1);
				return (NULL);
			}
			if (p->data->num_eat != -1)
			{
				if (p[p->i].eat_up == p->data->num_eat)
					count++;
				if (count == p->data->num)
				{
					pthread_mutex_lock(&p->data->print);
					return (NULL);
				}
			}
			//usleep(100);
		}
		//usleep(100);
	}
	usleep(500000);
	return (NULL);
}


t_phil 	*init(t_data *d) //вопрос к полям структуры
{
	t_phil *p;

	p = malloc(sizeof(t_phil) * d->num);
	p->i = -1;
	while (++p->i < d->num)
	{
		p[p->i].data = d;
		p[p->i].id = p->i;
		p[p->i].left = p->i;
		p[p->i].right = p->i + 1;
		if (p->i == d->num - 1)
			p[p->i].right = 0;
		p[p->i].last_meal = 0;
		p[p->i].is_eating = 0;
		p[p->i].eat_up = 0;
		p[p->i].threads = malloc(sizeof(pthread_t));
	}
	pthread_mutex_init(&p->data->print, NULL);
	d->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * d->num);
	p->i = -1;
	while (++p->i < d->num)
		pthread_mutex_init(&d->forks[p->i], NULL);
	return p;
}
void		parse_argv(char **av, t_data *d)
{
	d->num_eat = -1;
	d->num = atoi_philo(av[1]);
	d->time_to_die = atoi_philo(av[2]);
	d->time_to_eat = atoi_philo(av[3]);
	d->time_to_sleep = atoi_philo(av[4]);
	if (av[5])
		d->num_eat = atoi_philo(av[5]);
	if (d->num > 200 || d->time_to_die < 60 || d->time_to_sleep < 60)
		error("wrong arguments", 1);
}
int			main(int ac, char **av)
{
	t_data d;
	t_phil *p;

	if (ac == 5 || ac == 6)
	{
		p = NULL;
		parse_argv(av, &d);
		p = init(&d);
		start_threads(&d, p);
		//clear_leaks(p, &d);
	}
	else
		error("wrong arguments", 1);
	getchar();
	return (0);
}

