/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 22:29:22 by cwindom           #+#    #+#             */
/*   Updated: 2021/03/23 10:47:39 by maria            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

static int	exit_kill(t_phil *p)
{
	long	now;
	long	time_die;

	now = gettime();
	time_die = p[p->i].last_meal + p->data->time_to_die;
	if (now > time_die && !p[p->i].is_eating)
	{
		sem_wait(p->data->print);
		printf("%ld %d died\n", gettime() - p->data->t_s, p[p->i].id + 1);
		return (1);
	}
	return (0);
}

static int	exit_eat(t_phil *p)
{
	if (p->data->count_eat == p->data->num)
	{
		sem_wait(p->data->print);
		return (1);
	}
	return (0);
}

//void		*start_threads(t_data *d, t_phil *p)
//{
//	d->eat = 0;
//	p->i = -1;
//	while (++p->i < d->num)
//	{
//		pthread_create(p[p->i].threads, NULL, &lifetime, (void *)&p[p->i]);
//		usleep(200);
//		pthread_detach(*p[p->i].threads);
//	}
//	while (!d->eat)
//	{
//		p->i = -1;
//		d->count_eat = 0;
//		while (++p->i < p->data->num)
//		{
//			if (exit_kill(p) == -1)
//				return (NULL);
//			if (p->data->num_eat != -1)
//			{
//				if (p[p->i].eat_up == p->data->num_eat)
//					d->count_eat++;
//				d->eat = exit_eat(p);
//			}
//		}
//	}
//	return (NULL);
//}

void kill_processes(t_data *d, t_phil *p)
{
	p->i = -1;
	while (++p->i < d->num)
		kill(p[p->i].pid, SIGKILL);
}

int process(t_data *d, t_phil *p)
{
	int status;
	int isDead;

	p->i = -1;
	isDead = 0;
	d->count_eat = 0;
	while (++p->i < d->num)
	{
		p[p->i].pid = fork();
		if (p[p->i].pid < 0)//ошибка, выходим
			return (1);
		else if (p[p->i].pid == 0)//дочка, идем в функцию жизни философа как-то, но как?
		{
			pthread_create(p[p->i].threads, NULL, (void *)lifetime, &p[p->i]);
			while(!isDead)
			{
				usleep(100);
				isDead = exit_kill(p);
				if (p->data->num_eat != -1 && !isDead)
					if (p[p->i].eat_up == p->data->num_eat)
						d->count_eat++;
				isDead = exit_eat(p);
			}
			kill_processes(d, p);
		}
	}
	waitpid(-1, &status, 0);
	if (d->num_eat != -1 && WIFEXITED(status) && WEXITSTATUS(status) != 0)
		//остановиться есть
		printf("hz chto pisat'");
}
