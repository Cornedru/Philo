#include <unistd.h>
#include <stdlib.h>
#include "../include/philo.h"

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

void *routine(void *param)
{
    t_philo *philo;
    philo = (t_philo *)param;
    t_data *data = philo->data;
	(void)data;
	int i = 0;
    int active;
	active = 1;
	while (i < 10)
	// while (check_death(philo))
	{
		if (philo->data->nbr_thread == 1)
		{
			print_statut(philo, "has taken a fork");
			return (0);
		}
		philo_eat(philo);
		i++;
	}
    return (0);
}

