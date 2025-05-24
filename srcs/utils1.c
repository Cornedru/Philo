/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndehmej <ndehmej@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 01:28:40 by ndehmej           #+#    #+#             */
/*   Updated: 2025/05/21 01:50:00 by ndehmej          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"
#include <stdlib.h>
#include <unistd.h>

int	check_death(t_data *data)
{
	int	died;

	pthread_mutex_lock(&data->death_mutex);
	died = data->is_die;
	pthread_mutex_unlock(&data->death_mutex);
	return (died);
}

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_statut(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_statut(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_statut(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_statut(philo, "has taken a fork");
	}
}

void	update_meal_status(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = time_in_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
}

void	philo_eat(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	if (check_death(data))
		return ;
	take_forks(philo);
	if (check_death(data))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return ;
	}
	print_statut(philo, "is eating");
	update_meal_status(philo);
	ft_usleep(data->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

int	destroy_simu(t_data *data, t_philo *philo)
{
	int	i;

	pthread_mutex_lock(&data->death_mutex);
	data->stop_monitor = 1;
	pthread_mutex_unlock(&data->death_mutex);
	if (pthread_join(data->monitor_thread, NULL) != 0)
		return (1);
	i = 0;
	while (i < data->nbr_thread)
	{
		if (pthread_join(philo[i].thread, NULL) != 0)
			return (1);
		i++;
	}
	i = 0;
	while (i < data->nbr_thread)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&philo[i].meal_mutex);
		i++;
	}
	pthread_mutex_destroy(&data->write_mutex);
	pthread_mutex_destroy(&data->death_mutex);
	pthread_mutex_destroy(&data->eat_mutex);
	return (free(data->forks), free(data->philos), 0);
}
