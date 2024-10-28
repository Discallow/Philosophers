/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: discallow <discallow@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 14:35:04 by discallow         #+#    #+#             */
/*   Updated: 2024/10/25 03:57:12 by discallow        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	philo_died(t_data *data, int i)
{
	long	last_meal_time;

	mtx_actions(&data->philos[i].mtx, LOCK);
	last_meal_time = data->philos[i].last_meal_time;
	mtx_actions(&data->philos[i].mtx, UNLOCK);
	if (!process_bool(&data->sem_data, READ, &data->philos->philos_full)
		&& gettime(MILLISECOND) - last_meal_time > (data->time_to_die / 1000))
	{
		write_message(&data->philos[i], DIE);
		sem_actions(&data->sem_write, WAIT, NULL, 0);
		return (1);
	}
	return (0);
}

void	*monitor_thread(void *arg)
{
	t_philo	*philo;
	int		index;
	int		i;

	i = -1;
	philo = (t_philo *)arg;
	index = philo->place_in_table;
	while (1)
	{
		if (philo_died(philo->data, index - 1))
		{
			while (++i < philo->data->philo_num)
				sem_actions(&philo->data->sem_eat, POST, NULL, 0);
			return (NULL);
		}
	}
	return (NULL);
}

void	*kill_processes(void *arg)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)arg;
	sem_wait(philo->data->sem_death);
	usleep(1000);
	i = -1;
	while (++i < philo->data->philo_num)
		kill(philo[i].pid, SIGKILL);
	return (NULL);
}

void	*check_phillo_full(void *arg)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)arg;
	i = -1;
	while (++i < philo->data->philo_num)
		sem_actions(&philo->data->sem_eat, WAIT, NULL, 0);
	sem_post(philo->data->sem_death);
	return (NULL);
}

void	monitor_threads(t_data *data)
{
	thread_action(&data->check_phillo_full,
		check_phillo_full, data->philos, CREATE);
	pthread_detach(data->check_phillo_full);
	thread_action(&data->kill_philos, kill_processes, data->philos, CREATE);
	pthread_detach(data->kill_philos);
}
