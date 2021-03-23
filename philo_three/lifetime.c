/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lifetime.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 22:17:47 by cwindom           #+#    #+#             */
/*   Updated: 2021/03/23 20:30:04 by maria            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

void	wait_life(int time_to_wait)
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

void	eating(t_phil *p)
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

void	sleeping(t_phil *p)
{
	sem_wait(p->data->print);
	printf("%ld %d is sleeping\n", gettime() - p->data->t_s, p->id + 1);
	sem_post(p->data->print);
	wait_life(p->data->time_to_sleep);
}

void	thinking(t_phil *p)
{
	sem_wait(p->data->print);
	printf("%ld %d is thinking\n", gettime() - p->data->t_s, p->id + 1);
	sem_post(p->data->print);
}
