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
	int				death;
}					t_data;

typedef struct	s_phil
{
	int			i;
	int			id;
	int			left;
	int			right;

	int			full;
	int			has_eaten;
	time_t		last_meal;
	int			eating;
	pthread_t	*threads;
	//pthread_mutex_t	print;
	t_data		*data;
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
	printf("%ld %d %s", mygettimeofday() - p->data->time_start, p->id + 1, str);
	pthread_mutex_unlock(&p->data->print);
	//pthread_mutex_unlock(&phil->print);
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

	pthread_mutex_lock(&p->data->forks[p->left]);
	write_message(p, " has taken a fork\n");
	pthread_mutex_lock(&p->data->forks[p->right]);
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
	if (p->data->death)
	{
		pthread_mutex_unlock(&p->data->forks[p->left]);
		pthread_mutex_unlock(&p->data->forks[p->right]);
	}
}
static void	sleeping(t_phil *p)
{
	write_message(p, " is sleeping\n");
	usleep(p->data->time_to_sleep * 1000);
}



static void	*action(void *arg)
{
	t_phil		*p;

	p = (t_phil *)arg;
	p->last_meal = mygettimeofday();
	while (p->data->death)
	{
		if (p->data->death)
			eating(p);
		if (p->data->death)
			sleeping(p);
		if (p->data->death)
			write_message(p, " is thinking\n");
		//death ? eating(p) : 0;
		//death ? sleeping(p) : 0;
		//death ? write_message(p, " is thinking\n") : 0;
	}
	return (NULL);
}

void		*check_state(void *arg) //32 строки
{
	t_phil 			*p;
	int	count;

	p = (t_phil *)arg;
	while (1)
	{
		p->i = -1;
		count = 0;
		while (++p->i < p->data->num)
		{
			time_t	now;
			now = mygettimeofday();
			if ((!p[p->i].eating) && (now > (p[p->i].last_meal +p->data->time_to_die)))
			{
				pthread_mutex_lock(&p->data->print);
				p->data->death = 0;
				printf("%ld %d died\n", mygettimeofday() - p->data->time_start, p->id + 1);
				return (NULL);
			}
			if (p->data->num_eat)
			{
				if (p[p->i].full)
					count++;
				if (count == p->data->num)
				{
					pthread_mutex_lock(&p->data->print);
					printf("All philosophers ate %d times\n",p->data->num_eat);
					return (NULL);
				}
			}
		}
	}
}
void		*start_threads(t_data *d, t_phil *p)
{
	//pthread_t		tid;
	p->i = 0;
	d->time_start = mygettimeofday();
	while (p->i < d->num)
	{
		pthread_create(p[p->i].threads, NULL, action, (void *)&p[p->i]);
		pthread_detach(*p[p->i].threads);
		usleep(200);
		p->i++;
	}
//	pthread_create(&tid, NULL, &check_state, (void *)p);
//	pthread_join(tid, NULL);
	pthread_create(p->threads, NULL, &check_state, (void *)p);
	p->i = 0;
	while (p->i < d->num)
	{
		pthread_join(*p->threads, NULL);
		p->i++;
	}
	usleep(100000);
	return (NULL);
}


t_phil 	*init(t_data *d) //вопрос к полям структуры
{
	t_phil *p;

	p = malloc(sizeof(t_phil) * d->num);
	d->death = 1;
	p->i = 0;
	while (p->i < d->num)
	{
		p[p->i].data = d;
		p[p->i].id = p->i;
		p[p->i].left = p->i;
		p[p->i].right = (p->i + 1) % d->num;
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

