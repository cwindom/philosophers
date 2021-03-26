#include "philo_two.h"

static void	grabbing_forks(t_phil *p)
{
	sem_wait(p->data->waiter);
	sem_wait(p->data->forks);
	sem_wait(p->data->print);
	printf("%ld %d has taken a fork\n", gettime() - p->data->t_s, p->id + 1);
	sem_post(p->data->print);
	sem_wait(p->data->forks);
	sem_wait(p->data->print);
	printf("%ld %d has taken a fork\n", gettime() - p->data->t_s, p->id + 1);
	sem_post(p->data->print);
	sem_post(p->data->waiter);
}

static void	eating(t_phil *p)
{
	grabbing_forks(p);
	p->is_eating = 1;
	sem_wait(p->data->print);
	printf("%ld %d is eating\n", gettime() - p->data->t_s, p->id + 1);
	sem_post(p->data->print);
	p->last_meal = gettime();
	wait_life(p->data->time_to_eat);
	if (p->data->num_eat != -1)
		p->eat_up++;
	p->is_eating = 0;
	sem_post(p->data->forks);
	sem_post(p->data->forks);
}

static void	sleeping(t_phil *p)
{
	sem_wait(p->data->print);
	printf("%ld %d is sleeping\n", gettime() - p->data->t_s, p->id + 1);
	sem_post(p->data->print);
	wait_life(p->data->time_to_sleep);
}

static void	thinking(t_phil *p)
{
	sem_wait(p->data->print);
	printf("%ld %d is thinking\n", gettime() - p->data->t_s, p->id + 1);
	sem_post(p->data->print);
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
