/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: discallow <discallow@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 15:42:03 by discallow         #+#    #+#             */
/*   Updated: 2024/10/26 22:42:17 by discallow        ###   ########.fr       */
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

int	init_semaphores(t_data *data)
{
	sem_unlink(SEM_NAME);
	sem_unlink(WRITE_SEM_NAME);
	sem_unlink(FORKS_SEM_NAME);
	sem_unlink(MAX_PHILO_NAME);
	sem_unlink(EAT_SEM_NAME);
	sem_unlink(DEATH_SEM_NAME);
	if (sem_actions(&data->sem_write, OPEN, WRITE_SEM_NAME, 1)
		|| sem_actions(&data->forks->sem, OPEN, FORKS_SEM_NAME, data->philo_num)
		|| sem_actions(&data->sem_data, OPEN, SEM_NAME, 1)
		|| sem_actions(&data->sem_eat, OPEN, EAT_SEM_NAME, 0)
		|| sem_actions(&data->sem_death, OPEN, DEATH_SEM_NAME, 0)
		|| sem_actions(&data->sem_max_philo, OPEN,
			MAX_PHILO_NAME, data->philo_num - 1))
		return (1);
	return (0);
}

int	init_data(t_data *data)
{
	int		i;
	t_philo	*philo;

	i = -1;
	data->forks = ft_calloc(sizeof(t_fork), 1);
	data->philos = ft_calloc(sizeof(t_philo), data->philo_num);
	data->start = 0;
	if (init_semaphores(data))
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
