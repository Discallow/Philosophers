/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: discallow <discallow@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 15:42:03 by discallow         #+#    #+#             */
/*   Updated: 2024/10/22 17:00:33 by discallow        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*ft_calloc(size_t n, size_t size)
{
	void	*buffer;

	buffer = malloc(n * size);
	if (buffer == NULL)
	{
		printf(RED"Allocation failed"RESET"\n");
		return (NULL);
	}
	memset(buffer, 0, n * size);
	return (buffer);
}

void	choose_fork(t_philo *philo, t_fork *forks, int index)
{
	int	nbr_philos;

	nbr_philos = philo->data->philo_num;
	if (philo->place_in_table % 2 == 0)
	{
		philo->first_fork = &forks[index];
		philo->second_fork = &forks[(index + 1) % nbr_philos];
	}
	else
	{
		philo->first_fork = &forks[(index + 1) % nbr_philos];
		philo->second_fork = &forks[index];
	}
}

int	init_data(t_data *data)
{
	int		i;
	t_philo	*philo;

	i = -1;
	data->forks = ft_calloc(sizeof(t_fork), data->philo_num);
	data->philos = ft_calloc(sizeof(t_philo), data->philo_num);
	data->meal_end = false;
	data->num_philos_ready = 0;
	data->philos_ready = false;
	data->num_philos_full = 0;
	data->start = 0;
	if (mtx_actions(&data->mtx, INIT) || mtx_actions(&data->write_mtx, INIT))
		return (1);
	while (++i < data->philo_num)
	{
		if (mtx_actions(&data->forks[i].mtx, INIT)
			|| mtx_actions(&data->philos[i].mtx, INIT))
			return (1);
		philo = data->philos + i;
		philo->data = data;
		philo->place_in_table = i + 1;
		data->forks[i].fork_idx = i;
		choose_fork(philo, data->forks, i);
	}
	return (0);
}
