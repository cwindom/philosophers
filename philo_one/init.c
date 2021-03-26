#include "philo_one.h"

t_phil	*init(t_data *d)
{
	t_phil	*p;

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
	return (p);
}
