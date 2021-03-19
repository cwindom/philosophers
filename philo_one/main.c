#include "philo_one.h"
//void parse_argv(char **av, t_data *data)//возможно не нужно выделять здесь память но поч бы и нет
//{
//	int i;
//
//	i = 0;
//	data->num_of_philo = atoi_philo(av[1]);
//	data->time_to_die = atoi_philo(av[2]);
//	data->time_to_eat = atoi_philo(av[3]);
//	data->time_to_sleep = atoi_philo(av[4]);
//	if (av[5])
//		data->times_to_eat = atoi_philo(av[5]);
//	//debuh
//	printf("%d, %llu, %llu, %llu", data->num_of_philo, data->time_to_die,
//		   data->time_to_eat, data->time_to_sleep);
//	//debuh
//}
//
//pthread_mutex_t *init_mutex(t_data *data)
//{
//	int count;
//	pthread_mutex_t *ret;
//
//	count = 0;
//	if (!(ret = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * data->num_of_philo)))
//		error("in malloc");
//	while (count < data->num_of_philo)
//	{
//		pthread_mutex_init(&ret[count], NULL);
//		count++;
//	}
//	return (ret);
//}
//t_philo *init_phil(t_data *data, pthread_mutex_t *forks, pthread_mutex_t *print)
//{
//	t_philo *p;
//	p = malloc(sizeof(t_philo) * data->num_of_philo);
//	pthread_mutex_init(print, NULL);
//	int i = 0;
//	while (i < data->num_of_philo)
//	{
//		p[i].id = i;
//		p[i].data = data;
//		p[i].left_fork = i;
//		p[i].right_fork = (i + 1) % data->num_of_philo;
//		p[i].count_eat = 0;
//		i++;
//	}
//	return (p);
//}
//void *action(void *arg)
//{
//	t_philo *p;
//	p = (t_philo *)arg;
//
//	return (NULL);
//}
//void start(t_data *data, t_philo *phil)
//{
//	int i = 0;
//	while (i < data->num_of_philo)
//	{
//		pthread_create(&data->threads[i], NULL, action, &phil[i]);
//		usleep(300);
//		i++;
//	}
//}
//int mains(int ac, char **av)
//{
//	t_data data; //инфа
//	t_philo *phil; //массив философов
//	pthread_mutex_t *forks; //массив мьютексов вилок
//	pthread_mutex_t print; //один мьютекс на печать
//
//	phil = NULL;
//	if (ac == 5 || ac == 6)
//	{
//		parse_argv(av, &data);
//		forks = init_mutex(&data); //создать вилки
//		data.threads = malloc(sizeof(pthread_t) * data.num_of_philo);
//		phil = init_phil(&data, forks, &print);
//		data.time_on_start = mygettimeofday();
//		printf("\n%llu\n", data.time_on_start);
//		start(&data, phil);
//	}
//	else
//		error("wrong arguments");
//	return 0;
//}
