/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndehmej <ndehmej@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 03:46:15 by ndehmej           #+#    #+#             */
/*   Updated: 2025/05/24 23:06:20 by ndehmej          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"
#include <stdlib.h>
#include <unistd.h>

void	philo_think(t_philo *philo)
{
	if (check_death(philo->data))
		return ;
	print_statut(philo, "is thinking");
}

void	philo_sleep(t_philo *philo)
{
	if (check_death(philo->data))
		return ;
	print_statut(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep);
}

void	*philo(void *param)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)param;
	data = philo->data;
	if (philo->id % 2 == 0)
		usleep(1000);
	if (data->nbr_thread == 1)
	{
		philo_think(philo);
		print_statut(philo, "has taken a fork");
		ft_usleep(data->time_to_die);
		return (0);
	}
	while (!check_death(data))
	{
		philo_think(philo);
		philo_eat(philo);
		if (all_eat(data))
			break ;
		philo_sleep(philo);
	}
	return (0);
}
