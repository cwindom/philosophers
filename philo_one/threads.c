/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 22:29:22 by cwindom           #+#    #+#             */
/*   Updated: 2021/03/21 13:11:10 by maria            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

static int	exit_kill(t_phil *p)
{
	long	now;
	long	time_die;

	now = gettime();
	time_die = p[p->i].last_meal + p->data->time_to_die;
	if (now > time_die && !p[p->i].is_eating)
	{
		pthread_mutex_lock(&p->data->print);
		printf("%ld %d died\n", gettime() - p->data->t_s, p[p->i].id + 1);
		return (-1);
	}
	return (1);
}

static int	exit_eat(t_phil *p, int count_eat)
{
	if (count_eat == p->data->num)
	{
		pthread_mutex_lock(&p->data->print);
		return (-1);
	}
	return (1);
}

void		*start_threads(t_data *d, t_phil *p)
{
	d->count_eat = 0;
	d->t_s = gettime();
	p->i = -1;
	while (++p->i < d->num)
	{
		pthread_create(p[p->i].threads, NULL, &lifetime, (void *)&p[p->i]);
		usleep(200);
		pthread_detach(*p[p->i].threads);
	}
	while (1)
	{
		p->i = -1;
		while (++p->i < p->data->num)
		{
			if (exit_kill(p) == -1)
				return (NULL);
			if (p->data->num_eat != -1)
			{
				if (p[p->i].eat_up == p->data->num_eat)
					d->count_eat++;
				if (exit_eat(p, d->count_eat) == -1)
					return (NULL);
			}
		}
	}
}
