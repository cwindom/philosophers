#include "philo_three.h"

void	error(char *err, int exitcode)
{
	printf("Error: %s\n", err);
	exit(exitcode);
}

int		atoi_philo(const char *str)
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
		error("please enter correct arguments", 1);
	return (res);
}
