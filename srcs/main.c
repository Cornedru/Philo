#include <unistd.h>
#include "../include/philo.h"

int main(int ac, char **av)
{
	t_data	data;
	t_philo	*philos;

	if (check_args(ac, av, &data) != 0)
		return (1);
	philos = init_data(&data);
	if (!philos)
		return (1);
	if(init_mutex(&data))
		return(1);
	if (destroy_simu(&data, philos))
		return (1);
	return (0);

}