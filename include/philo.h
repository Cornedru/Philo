#ifndef PHILO_H
#define PHILO_H
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>


typedef	struct s_data
{

	// pthread_mutex_t	mutex;
	pthread_mutex_t *forks;
	pthread_mutex_t	write_mutex;
	int				time_to_die; 
	int				time_to_sleep;
	int				time_to_eat;
	int				time_to_start_to_eat;
	int 			time_to_end_to_eat;
	int 			count;
	int				is_die;
	// pthread_mutex_t
	pthread_mutex_t	*death_mutex;
	long long		start;
	long long 		end;
	int 			nbr_thread;
	struct s_philo	*philos;
	int				must_eat;
}	t_data;

typedef struct s_philo
{
    int             id;
    int             last_meal;
	int				time_since_last_meal;
	int				meals_eaten;
	pthread_t		thread;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    t_data          *data;
} t_philo;

void 				ft_error(char *str);
int 				ft_is_digit(char c);
int 				ft_is_number(const char *str);
int 				ft_atoi_ps(const char *str, int *error);
t_philo 			*init_data(t_data *data);
long long 			time_in_ms(void);
int 				check_args(int ac, char **av, t_data *data);
int 				init_mutex(t_data *data);
int 				destroy_simu(t_data *data, t_philo *philos);
void 				*routine(void *param);
void 				*monitor_routine(void *param);
int					if_somethings_bad(t_data *data);
void 				print_statut(t_philo *philos, char *msg);
void				update_last_meal(t_philo *philo);
int 				check_is_eating(t_data *data, t_philo *philos);
void 				philo_eat(t_philo *philo);
int 				check_death(t_philo	*philo);


#endif