#include "philo_one.h"

//int ft_strlen(char *str)
//{
//	int i;
//
//	i = 0;
//	while (str[i])
//		i++;
//	return (i);
//}
//void error(char *err)
//{
//	write(1, "Error: ", 7);
//	write(1, err, ft_strlen(err));
//	write(1, "\n", 1);
//	exit(1);
//}
//int	atoi_philo(const char *str)
//{
//	int res;
//	int i;
//
//	i = 0;
//	res = 0;
//	while ((str[i] >= '0') && (str[i] <= '9'))
//	{
//		res = res * 10 + (str[i] - '0');
//		i++;
//	}
//	if (str[i] && (str[i] < '0' || str[i] > '9'))
//		error("please enter correct arguments");
//	return (res);
//}