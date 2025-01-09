/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessadik <aessadik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 22:13:55 by aessadik          #+#    #+#             */
/*   Updated: 2025/01/09 22:25:26 by aessadik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_table	*table;

	if (ft_handle_parse_error(av, ac) == -1)
		return (1);
	table = malloc(sizeof(t_table));
	init_table(table, av);
	sim_dinner(table);
	clean(table);
	return 0;
}
