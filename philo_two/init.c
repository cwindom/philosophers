#include "philo_two.h"

t_phil	*init(t_data *d)
{
	t_phil	*p;

	p = malloc(sizeof(t_phil) * d->num);
	p->i = -1;
	while (++p->i < d->num)
	{
		p[p->i].data = d;
		p[p->i].id = p->i;
		p[p->i].last_meal = 0;
		p[p->i].is_eating = 0;
		p[p->i].eat_up = 0;
		p[p->i].threads = malloc(sizeof(pthread_t));
	}
	sem_unlink("/forks");
	d->forks = sem_open("/forks", O_CREAT, 0600, d->num);
	sem_unlink("/print");
	d->print = sem_open("/print", O_CREAT, 0600, 1);
	sem_unlink("/waiter");
	d->waiter = sem_open("/waiter", O_CREAT, 0600, 1);
	return (p);
}
