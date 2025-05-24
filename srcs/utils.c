/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndehmej <ndehmej@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 03:46:42 by ndehmej           #+#    #+#             */
/*   Updated: 2025/05/21 01:52:47 by ndehmej          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"
#include <stdlib.h>
#include <unistd.h>

unsigned long	time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	if (!&gettimeofday)
		return (-1);
	return ((((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000));
}

int	ft_atoi_ps(const char *str, int *error)
{
	long	res;
	int		sign;

	res = 0;
	sign = 1;
	if (*error == 1)
		return (0);
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '+' && str[1])
		str++;
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - '0');
		if (res * sign > 2147483647 || res * sign < -2147483648)
		{
			res = 0;
			break ;
		}
		str++;
	}
	if (*str || res == 0)
		*error = 1;
	return ((int)(res * sign));
}

void	print_statut(t_philo *philo, char *msg)
{
	unsigned long	time;
	t_data			*data;
	int				should_print;

	data = philo->data;
	should_print = 1;
	time = time_in_ms() - data->start;
	pthread_mutex_lock(&data->death_mutex);
	if (data->is_die && (msg[0] != 'd' || msg[1] != 'i' || msg[2] != 'e'
			|| msg[3] != 'd'))
		should_print = 0;
	pthread_mutex_unlock(&data->death_mutex);
	if (should_print)
	{
		pthread_mutex_lock(&data->write_mutex);
		printf("%lu %d %s\n", time, philo->id, msg);
		pthread_mutex_unlock(&data->write_mutex);
	}
}

void	ft_usleep(int time_ms)
{
	unsigned long	start;
	unsigned long	elapsed;

	start = time_in_ms();
	while (1)
	{
		elapsed = time_in_ms() - start;
		if (elapsed >= (unsigned long)time_ms)
			break ;
		usleep(500);
	}
}
