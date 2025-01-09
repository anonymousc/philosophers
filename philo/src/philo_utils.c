/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessadik <aessadik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 22:14:22 by aessadik          #+#    #+#             */
/*   Updated: 2025/01/09 22:16:42 by aessadik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time_m(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(size_t time)
{
	size_t	start;

	start = get_time_m();
	while (get_time_m() - start < time)
		usleep(60);
}

void	clean(t_table *table)
{
	pthread_mutex_destroy(&table->forks->fork);
	pthread_mutex_destroy(&table->philos->monitor_mutex);
	free(table);
	free(table->philos);
	free(table->forks);
}
