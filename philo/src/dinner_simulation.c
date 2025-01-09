/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_simulation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessadik <aessadik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 22:14:03 by aessadik          #+#    #+#             */
/*   Updated: 2025/01/09 22:18:39 by aessadik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	think(t_ph *philo)
{
	if (!philo->table->end_of_sim)
	{
		printf("%ld %d is thinking\n", get_time_m()
			- philo->table->start_of_sim, philo->id);
		usleep(150);
	}
}

static void	eat(t_ph *philo, atomic_long *last_meal)
{
	pthread_mutex_lock(&philo->fork_1->fork);
	if (!philo->table->end_of_sim)
		printf("%ld %d has taken a fork\n", get_time_m()
			- philo->table->start_of_sim, philo->id);
	pthread_mutex_lock(&philo->fork_2->fork);
	if (!philo->table->end_of_sim)
		printf("%ld %d has taken a fork\n", get_time_m()
			- philo->table->start_of_sim, philo->id);
	*last_meal = get_time_m();
	if (!philo->table->end_of_sim)
		printf("%ld %d is eating\n", get_time_m() - philo->table->start_of_sim,
			philo->id);
	philo->meals_eaten++;
	if (!philo->table->end_of_sim)
		ft_usleep(philo->table->time_to_eat);
	pthread_mutex_unlock(&philo->fork_2->fork);
	pthread_mutex_unlock(&philo->fork_1->fork);
}

static void	sleep_p(t_ph *philo)
{
	if (!philo->table->end_of_sim)
	{
		printf("%ld %d is sleeping\n", get_time_m()
			- philo->table->start_of_sim, philo->id);
		ft_usleep(philo->table->time_to_sleep);
	}
}

static void	*dinner(void *data)
{
	t_ph	*philo;

	philo = (t_ph *)data;
	philo->table->start_of_sim = get_time_m();
	philo->meals_eaten = 0;
	while (1)
	{
		if (philo->table->end_of_sim == true)
			return (NULL);
		eat(philo, &philo->last_time_eat);
		if (philo->meals_eaten == philo->table->num_of_meals)
			return (NULL);
		sleep_p(philo);
		think(philo);
	}
	return (NULL);
}

int	sim_dinner(t_table *table)
{
	int	i;

	i = 0;
	if (table->num_of_philos == 1)
	{
		table->start_of_sim = get_time_m();
		pthread_create(&table->philos[0].ph_id, NULL, single_threaded,
			table->philos);
		pthread_join(table->philos[0].ph_id, NULL);
	}
	else
	{
		while (i < table->num_of_philos)
		{
			pthread_create(&table->philos[i].ph_id, NULL, dinner, table->philos
				+ i);
			i++;
		}
		pthread_create(&table->monitor, NULL, monitor, table);
		i = -1;
		while (++i < table->num_of_philos)
			pthread_join(table->philos[i].ph_id, NULL);
		pthread_join(table->monitor, NULL);
	}
	return (0);
}
