/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: discallow <discallow@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 15:42:03 by discallow         #+#    #+#             */
/*   Updated: 2024/10/18 16:56:59 by discallow        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	thread_action(pthread_t *th, void *r(void *), void *data, t_code code)
{
	if (code == CREATE && pthread_create(th, NULL, r, data))
		return (printf(YELLOW"Error while creating thread."RESET"\n"));
	else if (code == JOIN && pthread_join(*th, NULL))
		return (printf(YELLOW"Error while joining thread."RESET"\n"));
	return (0);
}
int	mutex_actions(pthread_mutex_t *mtx, t_code code)
{
	if (code == INIT && pthread_mutex_init(mtx, NULL))
		return (printf(YELLOW"Error while starting mutex."RESET"\n"));
	else if (code == DESTROY && pthread_mutex_destroy(mtx))
		return (printf(YELLOW"Error while destroying mutex."RESET"\n"));
	else if (code == LOCK && pthread_mutex_lock(mtx))
		return (printf(YELLOW"Error while locking mutex."RESET"\n"));
	else if (code == UNLOCK && pthread_mutex_unlock(mtx))
		return (printf(YELLOW"Error while unlocking mutex."RESET"\n"));
	return (0);
}

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
	data->simulation_ended = false;
	data->num_philos_ready = 0;
	data->philos_ready = false;
	data->num_philos_full = 0;
	data->start = 0;
	if (mutex_actions(&data->mtx, INIT)
		|| mutex_actions(&data->write_mtx, INIT))
		return (1);
	while (++i < data->philo_num)
	{
		if (mutex_actions(&data->forks[i].mtx, INIT)
			|| mutex_actions(&data->philos[i].mtx, INIT))
			return (1);
		philo = data->philos + i;
		philo->data = data;
		philo->place_in_table = i + 1;
		data->forks[i].fork_idx = i;
		choose_fork(philo, data->forks, i);
	}
	return (0);
}
