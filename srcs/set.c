#include <unistd.h>
#include "../include/philo.h"


int	parse_args(int ac, char **av, t_data *data)
{
	int error;

	error = 0;
	if (ac != 5 && ac != 6)
		return (1);
	data->nbr_thread = ft_atoi_ps(av[1], &error);
	if (error != 0)
	{
		printf("hey moron enter an integer ... :\n");
		return (1);
	}
	data->time_to_die = ft_atoi_ps(av[2], &error);
	if (error != 0)
	{
		printf("hey moron enter an integer for time to die... :\n");
		return (1);
	}
	data->time_to_eat = ft_atoi_ps(av[3], &error);
	if (error != 0)
	{
		printf("hey moron enter an integer for time to eat... :\n");
		return (1);
	}
	data->time_to_sleep = ft_atoi_ps(av[4], &error);
	if (error != 0)
	{
		printf("hey moron enter an integer for time to sleep... :\n");
		return (1);
	}
	return (0);
}


