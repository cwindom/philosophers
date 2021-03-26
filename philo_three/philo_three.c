#include "philo_three.h"

void		clear_leaks(t_phil *p, t_data *d)
{
	sem_unlink("/fork");
	sem_unlink("/print");
	usleep(500000);
	if (p)
		free(p);
	p = NULL;
	sem_close(d->forks);
	sem_close(d->print);
}

void		parse_argv(char **av, t_data *d)
{
	d->num_eat = 0;
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
	t_data	d;
	t_phil	*p;

	if (ac == 5 || ac == 6)
	{
		p = NULL;
		parse_argv(av, &d);
		p = init(&d);
		d.t_s = gettime();
		process(&d, p);
		clear_leaks(p, &d);
	}
	else
		error("wrong arguments", 1);
	usleep(250000);
	return (0);
}
