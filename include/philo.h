/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndehmej <ndehmej@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 03:49:22 by ndehmej           #+#    #+#             */
/*   Updated: 2025/05/21 01:55:04 by ndehmej          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <limits.h>

typedef struct s_philo	t_philo;

typedef struct s_data
{
	int				nbr_thread;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	int				is_die;
	int				stop_monitor;
	int				all_eat;
	unsigned long	start;
	unsigned long	end;
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	eat_mutex;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_t		monitor_thread;
}	t_data;

typedef struct s_philo
{
	int				id;
	int				alive;
	int				meals_eaten;
	int				first_lap;
	unsigned long	last_meal;
	pthread_mutex_t	meal_mutex;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
}	t_philo;

void			*philo(void *param);

unsigned long	time_in_ms(void);
int				ft_atoi_ps(const char *str, int *error);
void			print_statut(t_philo *philo, char *msg);
void			ft_usleep(int time_ms);

t_philo			*init_data(t_data *data);
int				check_args(int ac, char **av, t_data *data);
t_philo			*init_forks(t_data *data);
t_philo			*init_philos(t_data *data);
int				create_threads(t_data *data);

void			*monitor_routine(void *param);
int				all_eat(t_data *data);
long long		get_time_since_last_meal(t_philo *philo);

void			philo_eat(t_philo *philo);
void			philo_sleep(t_philo *philo);
void			philo_think(t_philo *philo);
int				check_death(t_data *data);
int				destroy_simu(t_data *data, t_philo *philo);
void			take_forks(t_philo *philo);
#endif
