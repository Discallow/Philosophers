/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: discallow <discallow@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 14:35:04 by discallow         #+#    #+#             */
/*   Updated: 2024/10/18 14:12:22 by discallow        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isspace(int c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

long	ft_atol(const char *s)
{
	long	result;
	long	i;
	long	size;

	i = 0;
	result = 0;
	size = 0;
	while ((s[i] >= 9 && s[i] <= 13) || s[i] == 32)
		i++;
	if (s[i] == '+')
		i++;
	while (s[i] >= '0' && s[i] <= '9')
	{
		if (size > 10)
			return (invalid_input(s));
		result = result * 10 + (s[i] - '0');
		i++;
		size++;
	}
	if (result > INT_MAX)
		return (invalid_input(s));
	return (result);
}

static long	check_valid_input(char *str)
{
	long	i;
	long	num;

	i = 0;
	num = 0;
	while (str[i])
	{
		if (ft_isspace(str[i]))
			while (ft_isspace(str[i]))
				i++;
		else if (str[i] == '+')
			while(str[i] == '+')
				i++;
		if (ft_isdigit(str[i]))
			break ;
		if (str[i] && (ft_isspace(str[i]) || str[i] == '+'))
			i++;
		else
			return (invalid_input(str));
	}
	if (!str[i] || !ft_isdigit(str[i]))
		return (invalid_input(str));
	num = ft_atol(str + i);
	return (num);
}

int	parse(t_data *data, char **av)
{
	data->philo_num = check_valid_input(av[1]);
	data->time_to_die = check_valid_input(av[2]) * MILLISECOND;
	data->time_to_eat = check_valid_input(av[3]) * MILLISECOND;
	data->time_to_sleep = check_valid_input(av[4]) * MILLISECOND;
	if (av[5])
		data->min_times_to_eat = check_valid_input(av[5]);
	else
		data->min_times_to_eat = -1;
	if (data->philo_num == LONG_MIN || data->time_to_die == LONG_MIN
		|| data->time_to_eat == LONG_MIN || data->time_to_die == LONG_MIN
		|| data->min_times_to_eat == LONG_MIN)
		return (1);
	return (0);
}
