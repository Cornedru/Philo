#include <pthread.h>
#include  <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>



typedef	struct s_data
{

	pthread_mutex_t	mutex;
	pthread_t		*thread;
	pthread_mutex_t *forks;
	int				time_to_die; 
	int				time_to_sleep;
	int				time_to_eat;
	int				time_to_start_to_eat;
	int 			time_to_end_to_eat;
	int 			count;
	long long		start;
	long long 		end;
	int 			nbr_thread;
	struct s_philo	*philos;


}	t_data;

typedef struct s_philo
{
    int             id;
    int             last_meal;

    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    t_data          *data;
} t_philo;

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

long long time_in_ms(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	if (!&gettimeofday)
		return (-1);
	return (((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
}

long long start_simulation(t_data *data)
{

	data->start = time_in_ms();
		return (data->start);
}

long long end_simulation(t_data *data)
{
	data->end = time_in_ms();
		return (data->end);
}

void *routine(void *param)
{
    t_philo *philo;
    philo = (t_philo *)param;
    t_data *data = philo->data;


    printf("Philo %d is thinking\n", philo->id);    
    pthread_mutex_lock(philo->left_fork);
    printf("Philo %d takes left fork\n", philo->id);
    pthread_mutex_lock(philo->right_fork);
    printf("Philo %d takes right fork\n", philo->id);
    printf("Philo %d is eating\n", philo->id);
    philo->last_meal = time_in_ms();
    usleep(data->time_to_eat * 1000); 
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
    printf("Philo %d is sleeping\n", philo->id);
    usleep(data->time_to_sleep * 1000);
    
    return (0);
}

 int main (int ac, char **av)
{
	t_data data;
	int nbr_thread;
	int error;


	error = 0;
	
	if (ac != 5)
		return (1);
	nbr_thread = ft_atoi_ps(av[1], &error);
	if (error != 0)
	{
		printf("hey moron enter an integer ... :\n");
		return (1);
	}
	data.time_to_die = ft_atoi_ps(av[2], &error);
	if (error != 0)
	{
		printf("hey moron enter an integer for time to die... :\n");
		return (1);
	}
	data.time_to_eat = ft_atoi_ps(av[3] , &error);
	if (error != 0)
	{
		printf("hey moron enter an integer for time to eat... :\n");
		return (1);
	}
	data.time_to_sleep = ft_atoi_ps(av[4] , &error);
	if (error != 0)
	{
		printf("hey moron enter an integer for time to sleep... :\n");
		return (1);
	}
	data.thread = malloc(sizeof(pthread_t) * nbr_thread);
	if (!data.thread)
		return (1);
	data.forks = malloc(sizeof(pthread_mutex_t) * nbr_thread);
	if (!data.forks)
		return (1);
	data.philos = malloc(sizeof(t_philo) * nbr_thread);
	if (!data.philos)
		return (1);
	int i;	
	data.count = 0;
	if (pthread_mutex_init(&data.mutex, NULL) != 0)
		return (free(data.thread), 1);
	printf("start of simu :%lld\n", start_simulation(&data));
	i = 0;

	while (i < nbr_thread)
	{
		if (pthread_mutex_init(&data.forks[i], NULL) != 0)
			return (1);
		i++;
	}
	i  = 0;
	while (i < nbr_thread)
	{
		data.philos[i].id = i + 1;
		data.philos[i].left_fork = &data.forks[i];
		data.philos[i].right_fork = &data.forks[(i + 1) % nbr_thread];
		data.philos[i].data = &data;
		i++;
	}
	i = 0;
	while (i < nbr_thread)
	{
		if (pthread_create(&data.thread[i], NULL, routine, &data.philos[i]) != 0)
			return (1);
		i++;
	}
	i = 0;
	usleep(5000);
	while (i < nbr_thread)
	{
		if (pthread_join(data.thread[i++], NULL) != 0)
			return (1);
		else
		{
			printf("thread join :%d\n", i);
			printf("time in ms to thread join  : %lld\n", time_in_ms());
		}
	}
	free(data.thread);
	free(data.forks);
	free(data.philos);
	printf("end of simu :%lld\n", end_simulation(&data));
	return (0);
}

