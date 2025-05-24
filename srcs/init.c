/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndehmej <ndehmej@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 03:45:39 by ndehmej           #+#    #+#             */
/*   Updated: 2025/05/21 01:52:55 by ndehmej          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"
#include <stdlib.h>
#include <unistd.h>

int	init_mutex(t_data *data)
{
	if (pthread_mutex_init(&data->write_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&data->death_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->write_mutex);
		return (1);
	}
	if (pthread_mutex_init(&data->eat_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->write_mutex);
		pthread_mutex_destroy(&data->death_mutex);
		return (1);
	}
	return (0);
}

t_philo	*init_forks(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->nbr_thread);
	if (!data->forks)
		return (0);
	i = 0;
	while (i < data->nbr_thread)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			while (i > 0)
				pthread_mutex_destroy(&data->forks[--i]);
			free(data->forks);
			return (0);
		}
		i++;
	}
	return (data->philos);
}

t_philo	*init_philos(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->nbr_thread);
	if (!data->philos)
	{
		free(data->forks);
		return (0);
	}
	i = 0;
	while (i < data->nbr_thread)
	{
		if (pthread_mutex_init(&data->philos[i].meal_mutex, NULL) != 0)
			return (free(data->philos), free(data->forks), NULL);
		data->philos[i].alive = 1;
		data->philos[i].id = i + 1;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->nbr_thread];
		data->philos[i].data = data;
		data->philos[i].meals_eaten = 0;
		data->philos[i].first_lap = 1;
		i++;
	}
	return (data->philos);
}

t_philo	*init_data(t_data *data)
{
	if (init_mutex(data) != 0)
		return (0);
	if (init_forks(data) == 0)
	{
		pthread_mutex_destroy(&data->write_mutex);
		pthread_mutex_destroy(&data->death_mutex);
		pthread_mutex_destroy(&data->eat_mutex);
		return (0);
	}
	if (init_philos(data) == 0)
	{
		pthread_mutex_destroy(&data->write_mutex);
		pthread_mutex_destroy(&data->death_mutex);
		pthread_mutex_destroy(&data->eat_mutex);
		return (0);
	}
	data->start = time_in_ms();
	return (data->philos);
}

int	create_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbr_thread)
	{
		pthread_mutex_lock(&data->philos[i].meal_mutex);
		data->philos[i].last_meal = time_in_ms();
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
		if (pthread_create(&data->philos[i].thread, NULL, philo,
				&data->philos[i]) != 0)
			return (1);
		i++;
	}
	if (pthread_create(&data->monitor_thread, NULL, monitor_routine, data) != 0)
		return (1);
	return (0);
}
