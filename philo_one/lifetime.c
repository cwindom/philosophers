#include "philo_one.h"

static void	grabbing_forks(t_phil *p)
{
	pthread_mutex_lock(&p->data->forks[p->left]);
	pthread_mutex_lock(&p->data->print);
	printf("%ld %d has taken a fork\n", gettime() - p->data->t_s, p->id + 1);
	pthread_mutex_unlock(&p->data->print);
	pthread_mutex_lock(&p->data->forks[p->right]);
	pthread_mutex_lock(&p->data->print);
	printf("%ld %d has taken a fork\n", gettime() - p->data->t_s, p->id + 1);
	pthread_mutex_unlock(&p->data->print);
}

static void	eating(t_phil *p)
{
	grabbing_forks(p);
	p->is_eating = 1;
	pthread_mutex_lock(&p->data->print);
	printf("%ld %d is eating\n", gettime() - p->data->t_s, p->id + 1);
	pthread_mutex_unlock(&p->data->print);
	p->last_meal = gettime();
	wait_life(p->data->time_to_eat);
	if (p->data->num_eat != -1)
		p->eat_up++;
	p->is_eating = 0;
	pthread_mutex_unlock(&p->data->forks[p->right]);
	pthread_mutex_unlock(&p->data->forks[p->left]);
}

static void	sleeping(t_phil *p)
{
	pthread_mutex_lock(&p->data->print);
	printf("%ld %d is sleeping\n", gettime() - p->data->t_s, p->id + 1);
	pthread_mutex_unlock(&p->data->print);
	wait_life(p->data->time_to_sleep);
}

static void	thinking(t_phil *p)
{
	pthread_mutex_lock(&p->data->print);
	printf("%ld %d is thinking\n", gettime() - p->data->t_s, p->id + 1);
	pthread_mutex_unlock(&p->data->print);
}

void		*lifetime(void *arg)
{
	t_phil		*p;

	p = (t_phil *)arg;
	p->last_meal = gettime();
	while (1)
	{
		eating(p);
		sleeping(p);
		thinking(p);
	}
	return (NULL);
}
