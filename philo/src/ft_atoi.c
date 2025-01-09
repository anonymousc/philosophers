/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessadik <aessadik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 22:13:59 by aessadik          #+#    #+#             */
/*   Updated: 2025/01/09 22:16:19 by aessadik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

static int	ft_isspace(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

static int	ft_isntalpha(char c)
{
	return (!(c >= '0' && c <= '9'));
}

long	ft_atoi(char *str)
{
	long long	res;

	res = 0;
	while (str && ft_isspace(*str))
		str++;
	while (str[res])
	{
		if (ft_isntalpha(str[res]))
			return (0);
		res++;
	}
	res = 0;
	if (*str == '+')
		str++;
	while (*str && ft_isdigit(*str))
	{
		if (res > INT_MAX)
			return (write(2, "trying Some forbidden integer range\n", 37), 0);
		res = (res * 10) + (*str) - '0';
		str++;
	}
	return (res);
}
