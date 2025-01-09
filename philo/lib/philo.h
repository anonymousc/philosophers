/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessadik <aessadik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 22:14:34 by aessadik          #+#    #+#             */
/*   Updated: 2025/01/09 22:14:35 by aessadik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdatomic.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_table	t_table;

typedef struct s_forks
{
	int					id;
	pthread_mutex_t		fork;
}						t_fork;

typedef struct s_ph
{
	pthread_t			ph_id;
	t_fork				*fork_1;
	t_fork				*fork_2;
	int					id;
	atomic_int			meals_eaten;
	atomic_long			last_time_eat;
	pthread_mutex_t		monitor_mutex;
	t_table				*table;
}						t_ph;

typedef struct s_table
{
	int					num_of_philos;
	long				time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	pthread_t			monitor;
	atomic_int			num_of_meals;
	bool				end_of_sim;
	atomic_long			start_of_sim;
	t_ph				*philos;
	t_fork				*forks;
}						t_table;
/// utiles ///
long					ft_atoi(char *str);
long					get_time_m(void);
void					ft_usleep(size_t time);
/// exception ///
void					*single_threaded(void *data);
/// dinner sim ///
int						sim_dinner(t_table *table);
/// monitoring //
void					*monitor(void *data);
/// error handling ///
int						ft_handle_parse_error(char **av, int ac);
// init table ///
void					init_table(t_table *table, char **av);
void					clean(t_table *table);

#endif
