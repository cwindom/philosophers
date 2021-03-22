/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 22:17:58 by cwindom           #+#    #+#             */
/*   Updated: 2021/03/21 14:51:39 by maria            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

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

	if (ac == 5 || ac == 6)
	{
		p = NULL;
		parse_argv(av, &d);
		p = init(&d);
		start_threads(&d, p);
		if (p)
			free(p);
		p = NULL;
	}
	else
		error("wrong arguments", 1);
	usleep(500000);
	return (0);
}
