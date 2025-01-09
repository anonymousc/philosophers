/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exception.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessadik <aessadik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 22:14:13 by aessadik          #+#    #+#             */
/*   Updated: 2025/01/09 22:19:19 by aessadik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*single_threaded(void *data)
{
	t_ph	*philo;

	philo = (t_ph *)data;
	while (1)
	{
		pthread_mutex_lock(&philo->fork_1->fork);
		printf("%ld %d has taken a fork\n", get_time_m()
			- philo->table->start_of_sim, 1);
		pthread_mutex_unlock(&philo->fork_1->fork);
		ft_usleep(philo->table->time_to_die);
		printf("%ld %d died\n", get_time_m() - philo->table->start_of_sim,
			philo->table->philos[0].id);
		return (NULL);
	}
}
