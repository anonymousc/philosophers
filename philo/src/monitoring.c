/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessadik <aessadik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 22:14:17 by aessadik          #+#    #+#             */
/*   Updated: 2025/01/09 22:18:18 by aessadik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	check_death(t_ph *ph)
{
	if (get_time_m() - ph->last_time_eat >= ph->table->time_to_die)
		return (true);
	return (false);
}

void	*monitor(void *data)
{
	t_table	*table;
	int		i;

	table = data;
	pthread_mutex_lock(&table->philos->monitor_mutex);
	while (!table->end_of_sim)
	{
		ft_usleep(table->time_to_die);
		i = 0;
		while (!table->end_of_sim && i < table->num_of_philos)
		{
			if (table->philos->meals_eaten == table->num_of_meals)
				return (NULL);
			if (check_death(&table->philos[i]) && !table->end_of_sim)
			{
				table->end_of_sim = check_death(&table->philos[i]);
				printf("%ld %d died\n", get_time_m() - table->start_of_sim,
					table->philos[i].id);
				return (NULL);
			}
			i++;
		}
	}
	pthread_mutex_unlock(&table->philos->monitor_mutex);
	return (NULL);
}
