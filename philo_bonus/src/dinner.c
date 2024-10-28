/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: discallow <discallow@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:33:33 by discallow         #+#    #+#             */
/*   Updated: 2024/10/26 22:41:05 by discallow        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	eat(t_philo *philo)
{
	if (philo->data->philo_num == 1)
		sem_actions(&philo->data->sem_max_philo, POST, NULL, 0);
	sem_actions(&philo->data->sem_max_philo, WAIT, NULL, 0);
	sem_actions(&philo->data->forks->sem, WAIT, NULL, 0);
	write_message(philo, FORK);
	if (philo->data->philo_num == 1)
	{
		sem_actions(&philo->data->forks->sem, POST, NULL, 0);
		while (1)
			usleep(200);
	}
	sem_actions(&philo->data->forks->sem, WAIT, NULL, 0);
	write_message(philo, FORK);
	write_message(philo, EAT);
	mtx_actions(&philo->mtx, LOCK);
	philo->last_meal_time = gettime(MILLISECOND);
	mtx_actions(&philo->mtx, UNLOCK);
	philo->meal_counter++;
	improved_usleep(philo->data->time_to_eat);
	if (philo->data->min_times_to_eat > 0
		&& philo->data->min_times_to_eat == philo->meal_counter)
		sem_actions(&philo->data->sem_eat, POST, NULL, 0);
	sem_actions(&philo->data->forks->sem, POST, NULL, 0);
	sem_actions(&philo->data->forks->sem, POST, NULL, 0);
	sem_actions(&philo->data->sem_max_philo, POST, NULL, 0);
}

void	think(t_philo *philo, int *flag)
{
	long	time_to_think;

	if (*flag)
		*flag = 0;
	else
		write_message(philo, THINK);
	if (philo->data->philo_num % 2 == 0)
		return ;
	time_to_think = philo->data->time_to_eat * 2
		- philo->data->time_to_sleep;
	if (time_to_think < 0)
		time_to_think = 0;
	if (philo->place_in_table % 2)
		improved_usleep(time_to_think * 0.3);
}

void	routine(t_philo *philo)
{
	int		flag;

	flag = 1;
	mtx_actions(&philo->mtx, LOCK);
	philo->last_meal_time = gettime(MILLISECOND);
	mtx_actions(&philo->mtx, UNLOCK);
	think(philo, &flag);
	while (1)
	{
		eat(philo);
		write_message(philo, SLEEP);
		improved_usleep(philo->data->time_to_sleep);
		think(philo, &flag);
	}
}

int	start_dinner(t_data *data)
{
	int	i;

	i = -1;
	if (data->min_times_to_eat == 0)
		return (0);
	data->start = gettime(MILLISECOND);
	while (++i < data->philo_num)
	{
		data->philos[i].pid = fork();
		if (data->philos[i].pid < 0)
		{
			printf(YELLOW"Error while forking"RESET"\n");
			exit (1);
		}
		if (data->philos[i].pid == 0)
		{
			thread_action(&data->philos[i].monitor_thread, monitor_thread,
				&data->philos[i], CREATE);
			pthread_detach(data->philos[i].monitor_thread);
			routine(data->philos + i);
		}
	}
	monitor_threads(data);
	waitpid(-1, NULL, 0);
	return (0);
}
