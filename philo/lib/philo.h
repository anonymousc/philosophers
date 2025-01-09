#ifndef PHILO_H
#define PHILO_H

#include <sys/time.h>
#include <pthread.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdatomic.h>

typedef struct s_table t_table;

typedef struct s_forks 
{
	int id;
	pthread_mutex_t fork;
}	t_fork;

typedef struct s_ph
{
	pthread_t ph_id;
	t_fork  *fork_1;
	t_fork *fork_2;
	int	id;
	atomic_int meals_eaten;
	atomic_long last_time_eat;
	atomic_bool	is_full;
	t_table		*table;
}				t_ph;

typedef struct s_table
{
	int num_of_philos;
	long time_to_die;
	int time_to_eat;
	int time_to_sleep;
	pthread_t monitor;
	atomic_int num_ready_to_sync;
	atomic_int num_of_meals;
	atomic_bool end_of_sim;
	atomic_long start_of_sim;
	t_ph *philos;
	t_fork *forks;
}				t_table;

long ft_atoi(char *str);
long get_time_m();
int get_time_micro();
void ft_uslep(size_t time);

#endif
