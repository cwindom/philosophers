#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include <pthread.h>
// number_of_philosophers time_to_die
//time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
//number of philosophers = number of forks
//пронумеровать философов обязательно
//выводить на экран:
//timestamp_in_ms X has taken a fork
//timestamp_in_ms X is eating
//timestamp_in_ms X is sleeping
//timestamp_in_ms X is thinking
//timestamp_in_ms X died
//----------------------------------------------------
//memset, printf, malloc, free, write,
//usleep, gettimeofday, pthread_create,
//pthread_detach, pthread_join, pthread_mutex_init,
//pthread_mutex_destroy, pthread_mutex_lock,
//pthread_mutex_unlock
//----------------------------------------------------
//каждый философ это тред
//одна вилка между каждыми философами
//защищать состояние вилок с помощью мьютекса
//структура для философа, структура для инфы и общая структура с указателями на эти две стурктуры
//https://learnc.info/c/pthreads_deadlock.html
//typedef struct s_philo
//{
//    int num;
//} t_philo;
//void ft_error()
//{
//    write(1, "Error: wrong arguments\n", 23);
//    exit(1);
//}
//int main(int ac, char **av)
//{
//    struct timeval current_time;
//    time_t t;
//    time_t t1;
//    gettimeofday(&current_time, NULL);
//    t = current_time.tv_usec;
//    printf("%ld\n", t);
//    usleep(1000000);
//    t1 = current_time.tv_usec - t;
//    printf("%ld\n", t1);
//    if (ac == 4 || ac == 5)
//    {
//
//    }
//    else
//        ft_error();
//    return 0;
//}
#include <sys/wait.h>
#define PHT_SIZE 5

typedef struct philosopher_tag {
	const char *name;
	unsigned left_fork;
	unsigned right_fork;
} philosopher_t;


typedef struct table_tag {
	pthread_mutex_t forks[PHT_SIZE];
} table_t;


typedef struct philosopher_args_tag {
	const philosopher_t *philosopher;
	const table_t *table;
} philosopher_args_t;

pthread_mutex_t entry_point = PTHREAD_MUTEX_INITIALIZER;

void init_philosopher(philosopher_t *philosopher,
					  const char *name,
					  unsigned left_fork,
					  unsigned right_fork) {
	philosopher->name = name;
	philosopher->left_fork = left_fork;
	philosopher->right_fork = right_fork;
}

void init_table(table_t *table) {
	size_t i;
	for (i = 0; i < PHT_SIZE; i++) {
		pthread_mutex_init(&table->forks[i], NULL);
	}
}

void* eat(void *args) {
	philosopher_args_t *arg = (philosopher_args_t*) args;
	const philosopher_t *philosopher = arg->philosopher;
	const table_t *table = arg->table;
	unsigned rand;
	while (1)
	{
		printf("%s started dinner\n", philosopher->name);

		pthread_mutex_lock(&entry_point);
		pthread_mutex_lock(&table->forks[philosopher->left_fork]);
		rand_r(&rand);
		rand %= 1000;
		//sleep();
		pthread_mutex_lock(&table->forks[philosopher->right_fork]);
		pthread_mutex_unlock(&entry_point);

		printf("%s is eating after %d ms sleep\n", philosopher->name, rand);

		pthread_mutex_unlock(&table->forks[philosopher->right_fork]);
		pthread_mutex_unlock(&table->forks[philosopher->left_fork]);

		printf("%s finished dinner\n", philosopher->name);
	}
}

void main() {
	pthread_t threads[PHT_SIZE];
	philosopher_t philosophers[PHT_SIZE];
	philosopher_args_t arguments[PHT_SIZE];
	table_t table;
	size_t i;

	init_table(&table);

	init_philosopher(&philosophers[0], "Alice", 0, 1);
	init_philosopher(&philosophers[1], "Bob",   1, 2);
	init_philosopher(&philosophers[2], "Clark", 2, 3);
	init_philosopher(&philosophers[3], "Denis", 3, 4);
	init_philosopher(&philosophers[4], "Eugin", 4, 0);

	for (i = 0; i < PHT_SIZE; i++) {
		arguments[i].philosopher = &philosophers[i];
		arguments[i].table = &table;
	}

	for (i = 0; i < PHT_SIZE; i++) {
		pthread_create(&threads[i], NULL, eat, &arguments[i]);
	}

	for (i = 0; i < PHT_SIZE; i++) {
		pthread_join(threads[i], NULL);
	}
}

