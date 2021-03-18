#include "philo_one.h"

int ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}
void error(char *err)
{
	write(1, "Error: ", 7);
	write(1, err, ft_strlen(err));
	write(1, "\n", 1);
	exit(1);
}
time_t mygettimeofday()
{
	struct timeval	time;
	time_t			t;

	gettimeofday(&time, NULL);
	t = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (t);
}
int main_for_time()
{
	//for time
	time_t start_time = mygettimeofday();
	time_t current = mygettimeofday() - start_time;
	printf("%ld\n", current);
	usleep(1000); //1000 - это одна милисекунда
	current = mygettimeofday() - start_time;
	printf("%ld", current);
	//end time
	return 0;
}
int	atoi_philo(const char *str)
{
	int res;
	int i;

	i = 0;
	res = 0;
	while ((str[i] >= '0') && (str[i] <= '9'))
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	if (str[i] && (str[i] < '0' || str[i] > '9'))
		error("please enter correct arguments");
	return (res);
}
void parse_argv(char **av, t_data *data)
{
	data->num_of_philo = atoi_philo(av[1]);
	data->time_to_die = atoi_philo(av[2]);
	data->time_to_eat = atoi_philo(av[3]);
	data->time_to_sleep = atoi_philo(av[4]);
	if (av[5])
		data->times_to_eat = atoi_philo(av[5]);
	//debuh
	printf("%d, %ld, %ld, %ld", data->num_of_philo, data->time_to_die, data->time_to_eat, data->time_to_sleep);
	//debuh
}
void init(t_data *data)
{

}
void threads()
{

}
void clear(t_data *data)
{
	free(data->forks);
}
int main(int ac, char **av)
{
	int i;
	t_data *data;

	data = NULL;
	i = 0;
	if (!(data = (t_data *)malloc(sizeof (t_data))))
		error("in malloc");
    if (ac == 5 || ac == 6)
    {
		parse_argv(av, data);
		init(data);
		while(i < data->num_of_philo)
		{
			init(data);
			i++;
		}
		threads();
		clear(data);
    }
    else
        error("wrong arguments");
    return 0;
}
//с того сайта первые философы
//
//
//typedef struct s_all {
//	const t_philo *philosopher;
//	const t_data *table;
//} t_all;
//
////pthread_mutex_t entry_point = PTHREAD_MUTEX_INITIALIZER;
//
//void init_philosopher(t_philo *philosopher, int id, unsigned left_fork, unsigned right_fork)
//{
//	philosopher->id = id;
//	philosopher->left_fork = left_fork;
//	philosopher->right_fork = right_fork;
//}

//void init_table(t_data *table)
//{
//	size_t i;
//	for (i = 0; i < PHT_SIZE; i++)
//	{
//		pthread_mutex_init(&table->forks[i], NULL);
//	}
//}
//
//void* eat(void *args)
//{
//	t_all *arg = (t_all*) args;
//	const t_philo *philosopher = arg->philosopher;
//	const t_data *table = arg->table;
//	unsigned rand;
//	pthread_mutex_init(&table->entry_point, NULL);
//	while (1)
//	{
//		printf("%d started dinner\n", philosopher->id);
//
//		pthread_mutex_lock(&table->entry_point);
//		pthread_mutex_lock(&table->forks[philosopher->left_fork]);
//		rand_r(&rand);
//		rand %= 1000;
//		//sleep();
//		pthread_mutex_lock(&table->forks[philosopher->right_fork]);
//		pthread_mutex_unlock(&table->entry_point);
//
//		printf("%d is eating after %d ms sleep\n", philosopher->id, rand);
//
//		pthread_mutex_unlock(&table->forks[philosopher->right_fork]);
//		pthread_mutex_unlock(&table->forks[philosopher->left_fork]);
//
//		printf("%d finished dinner\n", philosopher->id);
//	}
//}
//
//void main()
//{
//	int ac = 5;
//	char **av[6];
//	av[0] = "no";
//	av[1] = "5"; //number_of_philosophers
//	av[2] = "800"; //time_to_die
//	av[3] = "200"; //time_to_eat
//	av[4] = "200"; //time_to_sleep
//	av[5] = "7"; //number_of_times_each_philosopher_must_eat
//	int number_philo = atoi(av[1]);
//	//парсинг и все такое
//	//проверить количество и качество аргументов
//
//	t_all all;
//	t_philo philo;
//	t_data data;
//	int count = 0; //количество философов
//
////	pthread_t threads[number_philo];
////	t_philo philosophers[number_philo];
////	t_all arguments[number_philo];
////	t_data table;
//	size_t i;
//	init_table(&table);
//	init_philosopher(&philosophers[0], 1, 0, 1);
//	init_philosopher(&philosophers[1], 2,   1, 2);
//	init_philosopher(&philosophers[2], 3, 2, 3);
//	init_philosopher(&philosophers[3], 4, 3, 4);
//	init_philosopher(&philosophers[4], 5, 4, 0);
//	for (i = 0; i < number_philo; i++)
//	{
//		arguments[i].philosopher = &philosophers[i];
//		arguments[i].table = &table;
//	}
//	for (i = 0; i < number_philo; i++)
//	{
//		pthread_create(&threads[i], NULL, eat, &arguments[i]);
//	}
//	for (i = 0; i < number_philo; i++)
//	{
//		pthread_join(&threads[i], NULL);
//	}
//}

