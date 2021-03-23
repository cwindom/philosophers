/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 22:18:09 by cwindom           #+#    #+#             */
/*   Updated: 2021/03/22 23:22:04 by maria            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_THREE_H
# define PHILO_THREE_H

# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <stdio.h>
# include <pthread.h>
# include <semaphore.h>
#include <signal.h>

typedef struct		s_data
{
	int				num;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_eat;
	int				count_eat;
	int 			eat;
	long			t_s;
	sem_t			*print;
	sem_t			*forks;
	sem_t			*waiter;
}					t_data;

typedef struct		s_phil
{
	int				i;
	int				id;
	int				eat_up;
	long			last_meal;
	int				is_eating;
	pid_t 			pid;
	pthread_t		*threads;
	t_data			*data;
}					t_phil;

long				gettime(void);
void				error(char *err, int exitcode);
int					atoi_philo(const char *str);
t_phil				*init(t_data *d);
int				lifetime(void *arg);
void				wait_life(int time_to_wait);
int process(t_data *d, t_phil *p);

#endif
