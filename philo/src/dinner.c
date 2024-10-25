/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: discallow <discallow@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:33:33 by discallow         #+#    #+#             */
/*   Updated: 2024/10/25 03:39:24 by discallow        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat(t_philo *philo)
{
	mtx_actions(&philo->first_fork->mtx, LOCK);
	write_message(philo, FORK);
	if (philo->data->philo_num == 1)
	{
		while (!process_bool(&philo->data->mtx, READ,
				&philo->data->meal_end))
			usleep(200);
		mtx_actions(&philo->first_fork->mtx, UNLOCK);
		return ;
	}
	mtx_actions(&philo->second_fork->mtx, LOCK);
	write_message(philo, FORK);
	write_message(philo, EAT);
	mtx_actions(&philo->mtx, LOCK);
	philo->last_meal_time = gettime(MILLISECOND);
	mtx_actions(&philo->mtx, UNLOCK);
	philo->meal_counter++;
	improved_usleep(philo->data->time_to_eat, philo->data);
	if (philo->data->min_times_to_eat > 0
		&& philo->data->min_times_to_eat == philo->meal_counter)
		process_bool(&philo->mtx, CHANGE, &philo->philos_full);
	mtx_actions(&philo->first_fork->mtx, UNLOCK);
	mtx_actions(&philo->second_fork->mtx, UNLOCK);
}

void	think(t_philo *philo, long *flag)
{
	if (process_bool(&philo->data->mtx, READ, &philo->data->meal_end))
		return ;
	if (process_long(&philo->data->mtx, READ, flag))
	{
		mtx_actions(&philo->data->mtx, LOCK);
		*flag = 0;
		mtx_actions(&philo->data->mtx, UNLOCK);
	}
	else
		write_message(philo, THINK);
	if (philo->data->philo_num % 2 == 0)
		return ;
	if (philo->place_in_table % 2)
		improved_usleep((philo->data->time_to_eat * 2
				- philo->data->time_to_sleep) * 0.3, philo->data);
}

void	*routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;
	long	flag;

	flag = 1;
	philo = (t_philo *)arg;
	data = philo->data;
	wait_all_threads(data);
	mtx_actions(&philo->mtx, LOCK);
	philo->last_meal_time = gettime(MILLISECOND);
	mtx_actions(&philo->mtx, UNLOCK);
	think(philo, &flag);
	process_long(&philo->data->mtx, CHANGE, &philo->data->num_philos_ready);
	while (1)
	{
		if (process_bool(&philo->data->mtx, READ, &philo->data->meal_end)
			|| process_bool(&philo->mtx, READ, &philo->philos_full))
			return (NULL);
		eat(philo);
		write_message(philo, SLEEP);
		improved_usleep(philo->data->time_to_sleep, philo->data);
		think(philo, &flag);
	}
	return (NULL);
}

int	start_dinner(t_data *data)
{
	int	i;

	i = -1;
	if (data->min_times_to_eat == 0)
		return (0);
	data->start = gettime(MILLISECOND);
	while (++i < data->philo_num)
		thread_action(&data->philos[i].philo_thread, routine,
			&data->philos[i], CREATE);
	process_bool(&data->mtx, CHANGE, &data->philos_ready);
	thread_action(&data->monitor_thread, monitor_thread, data, CREATE);
	i = -1;
	while (++i < data->philo_num)
		thread_action(&data->philos[i].philo_thread, NULL, NULL, JOIN);
	process_bool(&data->mtx, CHANGE, &data->meal_end);
	thread_action(&data->monitor_thread, NULL, NULL, JOIN);
	return (0);
}
