/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndehmej <ndehmej@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 03:46:04 by ndehmej           #+#    #+#             */
/*   Updated: 2025/05/24 23:05:11 by ndehmej          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"
#include <stdlib.h>
#include <unistd.h>

long long	get_time_since_last_meal(t_philo *philo)
{
	long long	last_meal;

	pthread_mutex_lock(&philo->meal_mutex);
	last_meal = time_in_ms() - philo->last_meal;
	pthread_mutex_unlock(&philo->meal_mutex);
	return (last_meal);
}

int	all_eat(t_data *data)
{
	int	i;
	int	count;
	int	result;

	if (data->must_eat < 0)
		return (0);
	count = 0;
	i = 0;
	while (i < data->nbr_thread)
	{
		pthread_mutex_lock(&data->philos[i].meal_mutex);
		if (data->philos[i].meals_eaten >= data->must_eat)
			count++;
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
		i++;
	}
	pthread_mutex_lock(&data->eat_mutex);
	if (count == data->nbr_thread)
		data->all_eat = 1;
	result = data->all_eat;
	pthread_mutex_unlock(&data->eat_mutex);
	return (result);
}

void	*verif_dead(t_data *data, int i)
{
	pthread_mutex_lock(&data->death_mutex);
	if (!data->is_die)
	{
		data->is_die = 1;
		pthread_mutex_unlock(&data->death_mutex);
		print_statut(&data->philos[i], "died");
	}
	else
		pthread_mutex_unlock(&data->death_mutex);
	return (NULL);
}

void	*monitor_routine(void *param)
{
	t_data	*data;
	int		i;

	data = (t_data *)param;
	while (1)
	{
		i = -1;
		while (++i < data->nbr_thread)
		{
			if (get_time_since_last_meal(&data->philos[i]) > data->time_to_die)
				return (verif_dead(data, i));
		}
		pthread_mutex_lock(&data->death_mutex);
		if (data->stop_monitor || all_eat(data))
			return (pthread_mutex_unlock(&data->death_mutex), NULL);
		pthread_mutex_unlock(&data->death_mutex);
		usleep(1000);
	}
	return (0);
}
