/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lifetime.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 22:17:47 by cwindom           #+#    #+#             */
/*   Updated: 2021/03/21 14:51:39 by maria            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

static void	wait_life(int time_to_wait)
{
	long start_eat;
	long finish_eat;

	start_eat = gettime();
	finish_eat = start_eat;
	while ((finish_eat - start_eat) < time_to_wait)
	{
		usleep(10);
		finish_eat = gettime();
	}
	usleep(10);
}

static void	eating(t_phil *p)
{
	sem_wait(p->data->forks);
	sem_wait(p->data->print);
	printf("%ld %d has taken a fork\n", gettime() - p->data->t_s, p->id + 1);
	sem_post(p->data->print);
	sem_wait(p->data->forks);
	sem_wait(p->data->print);
	printf("%ld %d has taken a fork\n", gettime() - p->data->t_s, p->id + 1);
	sem_post(p->data->print);
	p->is_eating = 1;
	sem_wait(p->data->print);
	printf("%ld %d is eating\n", gettime() - p->data->t_s, p->id + 1);
	sem_post(p->data->print);
	p->last_meal = gettime();
	wait_life(p->data->time_to_eat);
	sem_post(p->data->forks);
	sem_post(p->data->forks);
	p->is_eating = 0;
	if (p->data->num_eat != -1)
		p->eat_up++;
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
