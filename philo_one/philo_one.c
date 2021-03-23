/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 22:17:58 by cwindom           #+#    #+#             */
/*   Updated: 2021/03/22 15:15:09 by maria            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void		clear_leaks(t_phil *p, t_data *d)
{
	if (d->forks)
		free(d->forks);
	usleep(500000);
	if (p)
	{
		p->i = -1;
		while (++p->i < d->num)
		{
			if(p[p->i].threads)
			free(p[p->i].threads);
		}
		free(p);
	}
	p = NULL;
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
	t_data	d;
	t_phil	*p;
	int		i;

	if (ac == 5 || ac == 6)
	{
		p = NULL;
		parse_argv(av, &d);
		p = init(&d);
		d.t_s = gettime();
		start_threads(&d, p);
		i = -1;
		pthread_mutex_destroy(&d.print);
		if (d.forks)
			while (++i < d.num)
				pthread_mutex_destroy(&d.forks[i]);
		clear_leaks(p, &d);
	}
	else
		error("wrong arguments", 1);
	usleep(500000);
	return (0);
}
