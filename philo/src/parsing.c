/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessadik <aessadik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 22:14:19 by aessadik          #+#    #+#             */
/*   Updated: 2025/01/09 22:19:09 by aessadik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	input_validation(char **av)
{
	int	i;

	i = 1;
	while (av[i])
	{
		if (ft_atoi(av[i]) <= 0)
			return (av = NULL, 2);
		i++;
	}
	return (0);
}

static int	parser(char **av, int ac)
{
	if (ac < 4 || ac > 7)
		return (10);
	return (input_validation(av));
}

int	ft_handle_parse_error(char **av, int ac)
{
	int	philos;

	philos = parser(av, ac);
	if (philos == 10 || philos == 2)
		return (write(2, "Invalid Args\n", 13), -1);
	return (philos);
}
