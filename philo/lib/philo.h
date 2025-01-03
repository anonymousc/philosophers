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

typedef struct s_tbla t_tbla;

typedef struct s_forks 
{
	int id;
	pthread_mutex_t fork;
}t_fork;

typedef struct s_ph
{
	atomic_bool ready;
	t_tbla *tbla;
	pthread_t ph_id;
	t_fork  *fork_1;
	t_fork *fork_2;
	atomic_int meals_eaten;
	atomic_int last_time_eat;
	atomic_bool is_full;
	atomic_int	id;
}				t_ph;

typedef struct s_tbla
{
	atomic_int num_ready_to_sync;
	atomic_int start_of_sim;
	atomic_int num_of_philos;
	atomic_int time_to_die;
	atomic_int time_to_eat;
	atomic_int time_to_sleep;
	atomic_int num_of_meals;
	t_ph *philos;
	t_fork *forks;
	atomic_int to_exit;
	atomic_int to_die;
}				t_tbla;

long ft_atoi(char *str);
int get_time_m ();
int get_time_micro ();
void ft_uslep (size_t time);

#endif
