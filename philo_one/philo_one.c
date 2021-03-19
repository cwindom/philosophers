#include "philo_one.h"

void parse_argv(char **av, t_data *data, t_philo *phil)//возможно не нужно выделять здесь память но поч бы и нет
{
	int i;

	i = 0;
	data->num_of_philo = atoi_philo(av[1]);
	data->time_to_die = atoi_philo(av[2]);
	data->time_to_eat = atoi_philo(av[3]);
	data->time_to_sleep = atoi_philo(av[4]);
	if (av[5])
		data->times_to_eat = atoi_philo(av[5]);
	phil = (t_philo *)malloc(sizeof(t_philo) * data->num_of_philo);
	phil->data->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * phil->data->num_of_philo);
	while (i < phil->data->num_of_philo)
	{
		pthread_mutex_init(&phil->data->forks[i], NULL);
		i++;
	}
	//debuh
	printf("%d, %llu, %llu, %llu", data->num_of_philo, data->time_to_die,
		   data->time_to_eat, data->time_to_sleep);
	//debuh
}
void init_philo(t_data *data, t_philo *phil, pthread_mutex_t *forks, pthread_mutex_t *print)
{

}
void init(t_data *data, t_philo *phil, pthread_mutex_t *forks, pthread_mutex_t *print)
{
	data->threads = (pthread_t *)malloc((sizeof(pthread_t) * data->num_of_philo));
	init_philo(data, phil, forks, print);
}
void threads()
{

}
void clear(t_data *data)
{
}
pthread_mutex_t *init_mutex(t_data *data)
{
	int count;
	pthread_mutex_t *ret;

	count = 0;
	if (!(ret = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * data->num_of_philo)))
		error("in malloc");
	while (count < data->num_of_philo)
	{
		pthread_mutex_init(&ret[count], NULL);
		count++;
	}
	return (ret);
}
int main(int ac, char **av)
{
	t_data data; //инфа
	t_philo *phil; //массив философов
	pthread_mutex_t *forks; //массив мьютексов вилок
	pthread_mutex_t print; //один мьютекс на печать

	phil = NULL;
	if (ac == 5 || ac == 6)
	{
		parse_argv(av, &data, phil);
		forks = init_mutex(&data); //создать вилки
		init(&data, phil, forks, &print); //создание
		threads(); //сами треды и все что внутри и вся работа
		clear(&data); //очистить утечки((
	}
	else
		error("wrong arguments");
	return 0;
}
