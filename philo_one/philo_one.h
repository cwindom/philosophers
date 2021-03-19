#ifndef PHILO_ONE_H
#define PHILO_ONE_H

#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <stdio.h>
#include <pthread.h>

//typedef struct s_data
//{
//	int			num_of_philo;
//	uint64_t	time_to_die;
//	uint64_t	time_to_eat;
//	uint64_t	time_to_sleep;
//	uint64_t	time_on_start;
//	int			times_to_eat;
//	pthread_t	*threads; 			//дескриптор потока каждого для философа
//
//									//подумать
//	pthread_mutex_t	*forks;
//	pthread_mutex_t *left_fork;		//подумать как все-таки их хранить
//	pthread_mutex_t *right_fork;	//как массив
//									//подумать
//	pthread_mutex_t write_lock;
//	pthread_mutex_t	dead_lock;
//} t_data;
//
//typedef struct s_philo
//{
//	int		id;
//						//подумать
//	int		left_fork;	//это второй вариант хранения вилок
//	int		right_fork;	//подумать какой лучше
//						//подумать
//	//или здесь?
//	pthread_t	*threads;
//	int		count_eat;
//	t_data	*data;
//} t_philo;
//
//time_t mygettimeofday();
//int ft_strlen(char *str);
//void error(char *err);
//int	atoi_philo(const char *str);

#endif