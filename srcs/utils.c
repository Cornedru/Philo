#include <unistd.h>
#include <stdlib.h>
#include "../include/philo.h"

int ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 -*s2);
} 

void ft_putstr_fd(char *s, int fd)
{
	int i;

	i = 0;
	while (s[i] != '\0')
	{
		write (fd, &s[i], 1);
		i++;
	}
}


int ft_atoi_ps(const char *str, int *error)
{
	long res = 0;
	int sign = 1;
	
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
		{
			sign = -1;
			*error = 1;
		}
	}
	if (!*str)
		*error = 1;
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - '0');
		if (res * sign > 2147483647  || res * sign < -2147483648)
			*error = 1;
		str++;
	}
	if (*str)
		*error = 1;
	return ((int)(res * sign));
}

void ft_error(char *message)
{
	ft_putstr_fd(message, 2);
}

long long time_in_ms(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	if (!&gettimeofday)
		return (-1);
	return (((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
}

void *philo(void *param)
{
    t_philo *philo;
    philo = (t_philo *)param;
    t_data *data = philo->data;
	// int i = 0;

    int active;
	active = 0;
	philo->alive = 1;
	philo->first_lap = 1;
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = time_in_ms();
	philo->meals_eaten = 0;
	pthread_mutex_unlock(&philo->meal_mutex);
	// if (philo->id % 2 == 0 && philo->first_lap)
	// {	
	// 	philo->first_lap = 0;
	// 	usleep(1000);
	// }
	// else
	// {
		while (philo->alive == 1)
		{
				print_statut(philo, "is thinking");
				pthread_mutex_lock(&data->death_mutex);
				if (data->is_die == 1)
				{
					pthread_mutex_unlock(&data->death_mutex);
					break;
				}
				pthread_mutex_unlock(&data->death_mutex);
				if (philo->data->nbr_thread == 1)
				{
					print_statut(philo, "has taken a fork");
					// print_statut(philo, "is die");
					usleep(data->time_to_die * 1000);
					return (0);
				}
				philo_eat(philo);
				pthread_mutex_lock(&data->death_mutex);
				if (data->is_die == 1)
				{
					pthread_mutex_unlock(&data->death_mutex);
					break;
				}
				pthread_mutex_unlock(&data->death_mutex);
				// pthread_mutex_lock(&data->death_mutex);
				// if (data->is_die == 1 /*&& data->must_eat > 0*/)
				// {
			// 	pthread_mutex_unlock(&data->death_mutex);
			// 	break;
			// }
			// pthread_mutex_unlock(&data->death_mutex);
			// if (all_eat(philo->data))
			// 	break;
			
			// if (data->must_eat > 0)
			// {
			// 	pthread_mutex_lock(&philo->meal_mutex);
			// 	if (philo->meals_eaten >= data->must_eat)
			// 	{
			// 		pthread_mutex_unlock(&philo->meal_mutex);
			// 		break;
			// 	}
			// 	pthread_mutex_unlock(&philo->meal_mutex);
			// }

		// }
	}
    return (0);
}

