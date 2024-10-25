/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: discallow <discallow@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 15:42:03 by discallow         #+#    #+#             */
/*   Updated: 2024/10/25 03:18:13 by discallow        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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

int	init_data(t_data *data)
{
	int		i;
	t_philo	*philo;

	i = -1;
	data->forks = ft_calloc(sizeof(t_fork), 1);
	data->philos = ft_calloc(sizeof(t_philo), data->philo_num);
	data->meal_end = false;
	data->num_philos_full = 0;
	data->start = 0;
	data->max_philo_name = "/max_philo_sem";
	data->sem_name = "/main_sem";
	data->write_sem_name = "/write_sem";
	data->forks->sem_name = "/forks_sem";
	data->eat_sem_name = "/eat_sem";
	data->sem_death_name = "/death_sem";
	data->status = 0;
	if (sem_actions(&data->sem, OPEN, data->sem_name, 1)
		|| sem_actions(&data->write_sem, OPEN, data->write_sem_name, 1)
		|| sem_actions(&data->forks->sem, OPEN, data->forks->sem_name, data->philo_num)
		|| sem_actions(&data->max_philo, OPEN, data->max_philo_name, data->philo_num - 1)
		|| sem_actions(&data->eat, OPEN, data->eat_sem_name, 0)
		|| sem_actions(&data->sem_death, OPEN, data->sem_death_name, 0))
		return (1);
	while (++i < data->philo_num)
	{
		if (mtx_actions(&data->philos[i].mtx, INIT))
			return (1);
		philo = data->philos + i;
		philo->data = data;
		philo->place_in_table = i + 1;
	}
	return (0);
}
