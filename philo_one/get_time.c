#include "philo_one.h"

//time_t mygettimeofday()
//{
//	struct timeval	time;
//	time_t			t;
//
//	gettimeofday(&time, NULL);
//	t = time.tv_sec * 1000 + time.tv_usec / 1000;
//	return (t);
//}
//static int main_for_time()
//{
//	//for time
//	time_t start_time = mygettimeofday();
//	time_t current = mygettimeofday() - start_time;
//	printf("%ld\n", current);
//	usleep(1000); //1000 - это одна милисекунда
//	current = mygettimeofday() - start_time;
//	printf("%ld", current);
//	//end time
//	return 0;
//}