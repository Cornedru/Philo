#include <unistd.h>
#include <stdlib.h>
#include "../include/philo.h"

unsigned long start_simulation(t_data *data)
{

	data->start = time_in_ms();
		return (data->start);
}

unsigned long end_simulation(t_data *data)
{
	data->end = time_in_ms();
		return (data->end);
}

void print_statut(t_philo *philos, char *msg)
{
	unsigned long time;
	t_data	*data;
	data = philos->data;

	time = time_in_ms() - data->start;
	pthread_mutex_lock(&data->write_mutex);
	printf("%lu %d %s\n", time, philos->id, msg);
	pthread_mutex_unlock(&data->write_mutex);
}

long long get_time_since_last_meal(t_philo *philo)
{
	long long last_meal;

	pthread_mutex_lock(&philo->meal_mutex);
	last_meal = time_in_ms() -  philo->last_meal;
	pthread_mutex_unlock(&philo->meal_mutex);
	return (last_meal);
}
int check_args(int ac, char **av, t_data *data)
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
	data->time_to_eat = ft_atoi_ps(av[3] , &error);
	if (error != 0)
	{
		printf("hey moron enter an integer for time to eat... :\n");
		return (1);
	}
	data->time_to_sleep = ft_atoi_ps(av[4] , &error);
	if (error != 0)
	{
		printf("hey moron enter an integer for time to sleep... :\n");
		return (1);
	}
	if (ac == 6)
	{
		data->must_eat = ft_atoi_ps(av[5] , &error);
		if (error != 0)
		{
			printf("hey moron enter an integer for must eat... :\n");
			return (1);
		}
	}
	return (0);
}

int init_mutex(t_data *data)
{
	int i;

	i = 0 ;

	if (pthread_mutex_init(&data->write_mutex, NULL) != 0)
		return (free(data->philos), free(data->forks), 1);
	if (pthread_mutex_init(&data->death_mutex, NULL) != 0)
		return (free(data->philos), free(data->forks), 1);
	return (0);
}
	
int create_threads(t_data *data)
{
	int i;

	i = 0;
	
	while (i < data->nbr_thread)
	{
		if (pthread_create(&data->philos[i].thread, NULL, philo, &data->philos[i]) != 0)
			return (1);
		i++;
	}
	if (pthread_create(&data->monitor_thread, NULL, monitor_routine, data) != 0)
		return (1);
	return (0);
}

t_philo* init_forks(t_data *data)
{
	int i;
	
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nbr_thread);
	if (!data->forks)
		return (NULL);
	i = 0;
	while (i < data->nbr_thread)
	{
		
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			while (i > 0)
				pthread_mutex_destroy(&data->forks[--i]);
			return (free(data->forks), NULL);
		}
		i++;
	}
	return (data->philos);
}
	
t_philo* init_philos(t_data *data)
{
	int i;
	
	data->philos = malloc(sizeof(t_philo) * data->nbr_thread);
	if (!data->philos)
		return (free(data->forks), NULL);
	i = 0;
	while (i < data->nbr_thread)
	{
		if (pthread_mutex_init(&data->philos[i].meal_mutex, NULL) != 0)
			return (NULL);
		// data->philos[i].alive = 1;
		data->philos[i].id = i + 1;
		data->philos[i].left_fork = &data->forks[i];
		if(data->nbr_thread != 1)
			data->philos[i].right_fork = &data->forks[(i + 1) % data->nbr_thread];
		data->philos[i].data = data;
		i++;
	}
	return (data->philos);
}

t_philo* init_data(t_data *data)
{
	printf("Initialisation des mutex\n");
	// if (pthread_mutex_init(&data->write_mutex, NULL) != 0 
	// || pthread_mutex_init(&data->death_mutex, NULL) != 0)
	// {
	// 	printf("Erreur des mutex\n");
	// 	// free(data->philos);
	// 	return NULL;
	// }
	if (init_mutex(data) != 0)
	{
		printf("Erreur des mutex\n");
		free(data->philos);
		return (NULL);
	}
	printf("Initialisation des forks\n");
	if (init_forks(data) == NULL)
	{
		printf("Erreur des forks\n");
		pthread_mutex_destroy(&data->write_mutex);
		pthread_mutex_destroy(&data->death_mutex);
		return NULL;
	}
	printf("Initialisation des philo\n");
	if (init_philos(data) == NULL)
	{
		printf("Erreur des philo\n");
		free(data->forks);
		pthread_mutex_destroy(&data->write_mutex);
		pthread_mutex_destroy(&data->death_mutex);
		return NULL;
	}
	data->start = time_in_ms();
	return (data->philos);
}


int destroy_simu(t_data *data, t_philo	*philo)
{
	int i;
	
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
	pthread_mutex_destroy(&data->write_mutex);
	pthread_mutex_destroy(&data->death_mutex);
	i = 0;
	while (i < data->nbr_thread)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&philo[i].meal_mutex);
		i++;
	}
	free(data->forks);
	free(data->philos);
	printf("end of simu :%lld\n\n", time_in_ms() - data->start);
	return (0);
}

int all_eat(t_data *data)
{
	int i;
	i = 0;
	int count;
	count = 0;

	while (i < data->nbr_thread)
	{
		pthread_mutex_lock(&data->philos[i].meal_mutex);
		if (data->philos[i].meals_eaten >= data->must_eat)
			count++;
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
		i++;
	}
	return (count == data->nbr_thread);
}	


void philo_eat(t_philo *philo)
{
    t_data *data = philo->data;

	// pthread_mutex_lock(&data->death_mutex);
	// if (data->is_die == 1)
	// {
	// 	pthread_mutex_unlock(&data->death_mutex);
	// 	return ;
	// }
	// pthread_mutex_unlock(&data->death_mutex);
	if (philo->id % 2 == 0)
	{
		usleep(200);
		pthread_mutex_lock(philo->right_fork);
		print_statut(philo, "has taken right fork");
		pthread_mutex_lock(philo->left_fork);
		print_statut(philo, "has taken left fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_statut(philo, "has taken left fork");
		pthread_mutex_lock(philo->right_fork);
		print_statut(philo, "has taken right fork");
	}

	printf("last meal :%lld\n", get_time_since_last_meal(philo));
	print_statut(philo, "is eating");
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal = time_in_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	usleep(data->time_to_eat * 1000);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	print_statut(philo, "is sleeping");
	usleep(data->time_to_sleep * 1000);
}



void *monitor_routine(void *param)
{
	t_data *data = (t_data *)param;
	int i;

	// usleep(100);
	while (1)
	{
		pthread_mutex_lock(&data->death_mutex);
        if (data->stop_monitor)
        {
            pthread_mutex_unlock(&data->death_mutex);
            break;
        }
        pthread_mutex_unlock(&data->death_mutex);
		i = 0;
		while (i < data->nbr_thread)
		{
			if (get_time_since_last_meal(&data->philos[i]) > data->time_to_die || 
					all_eat(data))
			{
				pthread_mutex_lock(&data->death_mutex);
				if (!data->is_die)
				{
					print_statut(&data->philos[i], "is died");
					data->is_die = 1;
				}
				pthread_mutex_unlock(&data->death_mutex);
				return (0);
			}
			i++;
		}
	}
	return (0);
}

