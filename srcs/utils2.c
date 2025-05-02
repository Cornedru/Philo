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
void	update_last_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->write_mutex);
	philo->last_meal = time_in_ms();
	pthread_mutex_unlock(&philo->data->write_mutex);
}

int check_args(int ac, char **av, t_data *data)
{
	int error;
	error = 0;
	
	if (ac != 5)
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
	return (0);
}

int init_mutex(t_data *data)
{
	int i;

	i = 0 ;

	while (i < data->nbr_thread)
	{
		data->philos[i].id = i + 1;
		data->philos[i].left_fork = &data->forks[i];
		if(data->nbr_thread != 1)
			data->philos[i].right_fork = &data->forks[(i + 1) % data->nbr_thread];
		data->philos[i].data = data;
		if (pthread_create(&data->philos[i].thread, NULL, routine, &data->philos[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}

t_philo* init_data(t_data *data)
{
	int i;

	// philos;
	i = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nbr_thread);
	if (!data->forks)
		return (NULL);
	data->philos = malloc(sizeof(t_philo) * data->nbr_thread);
	if (!data->philos)
		return (free(data->forks), NULL);
	// data->count = 0;
	if (pthread_mutex_init(&data->write_mutex, NULL) != 0)
		return (free(data->philos), free(data->forks), NULL);
	printf("start of simu :%lu\n", start_simulation(data));
	while (i < data->nbr_thread)
	{

		if (pthread_mutex_init(&data->forks[i++], NULL) != 0)
		{
			while (i > 0)
			{	
				pthread_mutex_destroy(&data->forks[--i]);
			}
			pthread_mutex_destroy(&data->write_mutex);
			return (free(data->forks), free(data->philos),NULL);
		}
	}
	return (data->philos);
}


int destroy_simu(t_data *data, t_philo	*philo)
{
	int i;
	i = 0;
	// (void)philo;
	usleep(50000);
	while (i < data->nbr_thread)
	{
		if (pthread_join(philo[i++].thread, NULL) != 0)
			return (1);
		else
		{
			printf("thread join :%d\n", i);
			printf("%lld : time in ms to thread join \n", time_in_ms() - data->start);

		}
	}
	// free(data->thread);
	free(data->forks);
	free(data->philos);
	printf("end of simu :%lld\n\n", time_in_ms() - data->start);
	return (0);
}

// int all_eat(t_data *data, t_philo	*philos)
// {
// 	int i;
// 	i = 0;
// 	int count;
// 	count = data->nbr_thread;

// 	while (i < data->nbr_thread)
// 	{
// 		if (data->philos[i].meals_eaten >= data->must_eat)
// 			count++;
// 		i++;
// 	}
// 	return (count);
// }	

int check_is_eating(t_data *data, t_philo *philo)
{

	int i;
	i = 0;
	// usleep(1000);
	// if (data->time_to_die < data->time_to_eat)
	while (i < data->nbr_thread)
	{
		if (time_in_ms() - philo->last_meal > data->time_to_eat)
		{
			print_statut(philo, "has taken fork");

			// printf("time last meal %lld\n", time_in_ms() - philo->last_meal);
			philo->data->is_die = 1;
			// exit(1);
		}
		i++;
	}
	return (0);
}

void philo_eat(t_philo *philo)
{
    t_data *data = philo->data;


	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_statut(philo, "has taken right fork for un philo paire");
		pthread_mutex_lock(philo->left_fork);
		print_statut(philo, "has taken left fork");	
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_statut(philo, "has taken right fork");
		pthread_mutex_lock(philo->left_fork);
		print_statut(philo, "has taken left fork");
	}
	// check_is_eating(data, philo);
	print_statut(philo, "is eating");
	philo->last_meal = time_in_ms();
	philo->meals_eaten++;
	usleep(data->time_to_eat * 1000);
	

	pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
	print_statut(philo, "is sleeping");
	usleep(data->time_to_sleep * 1000);

}

int check_death(t_philo	*philo)
{
	pthread_mutex_lock(philo->data->death_mutex);
	if (philo->data->is_die == 1)
	{
		pthread_mutex_unlock(philo->data->death_mutex);
		exit(1);
			// return (1);
	}
	pthread_mutex_unlock(philo->data->death_mutex);
	return (0);

}


