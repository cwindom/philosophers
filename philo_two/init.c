/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 22:17:35 by cwindom           #+#    #+#             */
/*   Updated: 2021/03/21 23:35:08 by maria            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		p[p->i].left = p->i;
		p[p->i].right = p->i + 1;
		if (p->i == d->num - 1)
			p[p->i].right = 0;
		p[p->i].last_meal = 0;
		p[p->i].is_eating = 0;
		p[p->i].eat_up = 0;
		p[p->i].threads = malloc(sizeof(pthread_t));
	}
	sem_unlink("/forks");
	d->forks = sem_open("/forks", O_CREAT, 0666, d->num);
	sem_unlink("/print");
	d->print = sem_open("/print", O_CREAT, 0666, 1);
	return (p);
}
