#ifndef PHILO_ONE_H
#define PHILO_ONE_H

#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <stdio.h>
#include <pthread.h>

typedef struct s_philo
{
	int id;
	int left_fork;
	int right_fork;
	int count_eat;
	int state; //состояние философа, 0 - ест, 1 - думает, 2 - спит, 3 - умер
} t_philo;

typedef struct s_data
{
	int		num_of_philo;
	time_t	time_to_die;
	time_t	time_to_eat;
	time_t	time_to_sleep;
	int		times_to_eat;
	time_t	each_philo_must_eat;
	pthread_mutex_t *forks;
	pthread_mutex_t entry_point;
	pthread_mutex_t write_lock;
	pthread_mutex_t dead_lock;
} t_data;

typedef struct s_all
{
	t_philo *phil;
	t_data *data;
} t_all;

#endif