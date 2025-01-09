/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessadik <aessadik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 22:14:30 by aessadik          #+#    #+#             */
/*   Updated: 2025/01/09 22:14:31 by aessadik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_forks(t_table *table, int pos)
{
	table->philos[pos].fork_1 = &table->forks[pos];
	table->philos[pos].fork_2 = &table->forks[(pos + 1) % table->num_of_philos];
	if (table->philos[pos].id % 2 == 0)
	{
		table->philos[pos].fork_1 = &table->forks[(pos + 1)
			% table->num_of_philos];
		table->philos[pos].fork_2 = &table->forks[pos];
	}
}

static void	ft_init_table(t_table **table, char **av)
{
	(*table)->num_of_philos = ft_atoi(av[1]);
	(*table)->time_to_die = ft_atoi(av[2]);
	(*table)->time_to_eat = ft_atoi(av[3]);
	(*table)->time_to_sleep = ft_atoi(av[4]);
	(*table)->end_of_sim = false;
	if (av[5])
		(*table)->num_of_meals = ft_atoi(av[5]);
	(*table)->philos = malloc(sizeof(t_ph) * (*table)->num_of_philos);
	(*table)->forks = malloc(sizeof(t_fork) * (*table)->num_of_philos);
}

void	init_table(t_table *table, char **av)
{
	int	i;

	i = 0;
	ft_init_table(&table, av);
	while (i < table->num_of_philos)
	{
		pthread_mutex_init(&table->forks[i].fork, NULL);
		table->forks[i].id = i;
		i++;
	}
	i = 0;
	while (i < table->num_of_philos)
	{
		table->philos[i].id = i + 1;
		table->philos[i].table = table;
		take_forks(table, i);
		i++;
	}
	pthread_mutex_init(&table->philos->monitor_mutex, NULL);
}
