/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndehmej <ndehmej@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 03:45:47 by ndehmej           #+#    #+#             */
/*   Updated: 2025/05/21 01:52:53 by ndehmej          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"
#include <stdlib.h>
#include <unistd.h>

int	check_args(int ac, char **av, t_data *data)
{
	int	error;

	error = 0;
	if (ac != 5 && ac != 6)
		return (printf("Error: Invalid number of arguments\n"), 1);
	data->nbr_thread = ft_atoi_ps(av[1], &error);
	data->time_to_die = ft_atoi_ps(av[2], &error);
	data->time_to_eat = ft_atoi_ps(av[3], &error);
	data->time_to_sleep = ft_atoi_ps(av[4], &error);
	if (error != 0)
		return (printf("Invalid args, put number between 0 and INT_MAX\n"), 1);
	data->must_eat = -1;
	if (ac == 6)
	{
		data->must_eat = ft_atoi_ps(av[5], &error);
		if (error != 0)
			return (printf("Invalid number of times philosopher must eat\n"),
				1);
	}
	data->is_die = 0;
	data->stop_monitor = 0;
	data->all_eat = 0;
	return (0);
}

void	data_pass(t_data *data)
{
	while (1)
	{
		pthread_mutex_lock(&data->death_mutex);
		if (data->is_die)
		{
			pthread_mutex_unlock(&data->death_mutex);
			return ;
		}
		pthread_mutex_unlock(&data->death_mutex);
		pthread_mutex_lock(&data->eat_mutex);
		if (data->must_eat >= 0 && data->all_eat)
		{
			pthread_mutex_unlock(&data->eat_mutex);
			return ;
		}
		pthread_mutex_unlock(&data->eat_mutex);
		usleep(5000);
	}
}

int	main(int ac, char **av)
{
	t_data	data;

	if (check_args(ac, av, &data) != 0)
		return (1);
	if (init_data(&data) == 0)
		return (free(data.forks), 1);
	if (create_threads(&data) != 0)
		return (1);
	data_pass(&data);
	if (destroy_simu(&data, data.philos) != 0)
		return (1);
	return (0);
}
