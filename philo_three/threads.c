#include "philo_three.h"

static void	*killer(void *arg)
{
	long	now;
	long	time_die;
	t_phil	*p;

	p = (t_phil *)arg;
	while (1)
	{
		now = gettime();
		time_die = p[p->i].last_meal + p->data->time_to_die;
		if (now > time_die && !p[p->i].is_eating)
		{
			sem_wait(p->data->print);
			printf("%ld %d died\n", gettime() - p->data->t_s, p[p->i].id + 1);
			kill(0, SIGINT);
		}
	}
}

static int	lifetime(t_phil *p)
{
	pthread_t	kil;

	pthread_create(&kil, NULL, &killer, p);
	p->last_meal = gettime();
	while (!p->data->num_eat || p->data->num_eat > p->eat_up)
	{
		eating(p);
		sleeping(p);
		thinking(p);
	}
	pthread_detach(kil);
	if ((p->data->num_eat || p->data->num_eat == p->eat_up))
	{
		sem_wait(p->data->print);
		exit(2);
	}
	return (1);
}

int			process(t_data *d, t_phil *p)
{
	int status;

	p->i = -1;
	while (++p->i < d->num)
	{
		p[p->i].pid = fork();
		if (p[p->i].pid < 0)
			return (1);
		else if (p[p->i].pid == 0)
			lifetime(&p[p->i]);
	}
	waitpid(-1, &status, 0);
	return (0);
}
