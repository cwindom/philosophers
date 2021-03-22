cmake_minimum_required(VERSION 3.17)
project(philo_three C)

set(CMAKE_C_STANDARD 99)

add_executable(philo_three get_time.c init.c lifetime.c utils.c philo_three.c threads.c)