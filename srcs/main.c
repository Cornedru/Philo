#include <unistd.h>
#include "../include/philo.h"


int main(int ac, char **av)
{
    t_data data;

    if (check_args(ac, av, &data) != 0)
        return (1);
    if (init_data(&data) == NULL)
        return (1);
    if (create_threads(&data) != 0)
        return (1);
    while (1)
    {
        pthread_mutex_lock(&data.death_mutex);
        if (data.is_die || (data.must_eat > 0 && all_eat(&data)))
        {
            pthread_mutex_unlock(&data.death_mutex);
            break;
        }
        pthread_mutex_unlock(&data.death_mutex);
        usleep(1000);
    }
    if (destroy_simu(&data, data.philos) != 0)
        return (1);
    return (0);
}

// int main(int ac, char **av)
// {
// 	t_data	data;
// 	t_philo	*philos;

// 	if (check_args(ac, av, &data) != 0)
// 		return (1);
// 	philos = init_data(&data);
// 	if (!philos)
// 		return (1);
// 	if(init_mutex(&data))
// 		return(1);
// 	if (destroy_simu(&data, philos))
// 		return (1);
// 	return (0);

// }