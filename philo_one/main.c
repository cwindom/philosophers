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

typedef struct s_philo
{
    int num;
} t_philo;
void ft_error()
{
    write(1, "Error: wrong arguments\n", 23);
    exit(1);
}
int main(int ac, char **av)
{
    struct timeval current_time;
    time_t t;
    time_t t1;
    gettimeofday(&current_time, NULL);
    t = current_time.tv_usec;
    printf("%ld\n", t);
    usleep(1000000);
    t1 = current_time.tv_usec - t;
    printf("%ld\n", t1);
//    if (ac == 4 || ac == 5)
//    {
//
//    }
//    else
//        ft_error();
    return 0;
}
