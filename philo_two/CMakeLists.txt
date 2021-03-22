cmake_minimum_required(VERSION 3.17)
project(philo_two C)

set(CMAKE_C_STANDARD 99)

add_executable(philo_two get_time.c init.c lifetime.c utils.c philo_two.c threads.c)