/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: discallow <discallow@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:33:33 by discallow         #+#    #+#             */
/*   Updated: 2024/10/18 17:09:23 by discallow        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	gettime(long time)
{
	struct timeval	start;
	long			elapsed;

	if (gettimeofday(&start, NULL))
	{
		printf(YELLOW"gettimeofday function failed."RESET"\n");
		return (LONG_MIN);
	}
	if (time == MICROSECOND)
		elapsed = (start.tv_sec * MICROSECOND) + start.tv_usec;
	else
		elapsed = (start.tv_sec * MILLISECOND) + (start.tv_usec / MILLISECOND);
	return (elapsed);
}
void	improved_usleep(long microseconds, t_data *data)
{
	long	start;
	long	cur;
	long	left;

	start = gettime(MICROSECOND);
	while (gettime(MICROSECOND) - start < microseconds)
	{
		if (read_change_bool(&data->mtx, READ, &data->simulation_ended))
			return ;
		cur = gettime(MICROSECOND) - start;
		left = microseconds - cur;
		if (left > MILLISECOND)
			usleep(left / 2);
		else
		{
			while (1)
				if (gettime(MICROSECOND) - start >= microseconds)
					break ;
		}
	}
}

void	wait_all_threads(t_data *data)
{
	while (1)
	{
		if (read_change_bool(&data->mtx, READ, &data->philos_ready))
			break ;
	}
}

void	write_message(t_philo *philo, t_message message)
{
	long	time;

	time = gettime(MILLISECOND) - philo->data->start;
	if (read_change_bool(&philo->data->mtx, READ, &philo->data->simulation_ended))
		return ;
	mutex_actions(&philo->data->write_mtx, LOCK);
	if (message == FORK)
		printf("%-6ld %d has taken a fork\n", time, philo->place_in_table);
	else if (message == EAT)
		printf("%-6ld %d is eating\n", time, philo->place_in_table);
	else if (message == SLEEP)
		printf("%-6ld %d is sleeping\n", time, philo->place_in_table);
	else if (message == THINK)
		printf("%-6ld %d is thinking\n", time, philo->place_in_table);
	else if (message == DIE)
		printf("%-6ld %d died\n", time, philo->place_in_table);
	mutex_actions(&philo->data->write_mtx, UNLOCK);
}

void	eat(t_philo *philo)
{
	mutex_actions(&philo->first_fork->mtx, LOCK);
	write_message(philo, FORK);
	mutex_actions(&philo->second_fork->mtx, LOCK);
	write_message(philo, FORK);
	write_message(philo, EAT);
	improved_usleep(philo->data->time_to_eat, philo->data);
	mutex_actions(&philo->mtx, LOCK);
	philo->last_meal_time = gettime(MILLISECOND);
	mutex_actions(&philo->mtx, UNLOCK);
	philo->meal_counter++;
	if (philo->data->min_times_to_eat > 0
		&& philo->data->min_times_to_eat == philo->meal_counter)
			read_change_bool(&philo->mtx, CHANGE, &philo->philos_full);
	mutex_actions(&philo->first_fork->mtx, UNLOCK);
	mutex_actions(&philo->second_fork->mtx, UNLOCK);
}

void	think(t_philo *philo)
{
	if (read_change_bool(&philo->data->mtx, READ, &philo->data->simulation_ended))
		return ;
	write_message(philo, THINK);
}
void	*routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	wait_all_threads(data);
	mutex_actions(&philo->mtx, LOCK);
	philo->last_meal_time = gettime(MILLISECOND);
	mutex_actions(&philo->mtx, UNLOCK);
	read_change_long(&philo->data->mtx, CHANGE, &philo->data->num_philos_ready);
	while (1)
	{
		if (read_change_bool(&philo->data->mtx, READ, &philo->data->simulation_ended)
			|| read_change_bool(&philo->mtx, READ, &philo->philos_full))
			return (NULL);
		think(philo);
		eat(philo);
		write_message(philo, SLEEP);
		improved_usleep(philo->data->time_to_sleep, philo->data);
	}
	return (NULL);
}

void	*monitor_thread(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		if (read_change_long(&data->mtx, READ, &data->num_philos_ready) == data->philo_num)
			break ;
	}
	while (!read_change_bool(&data->mtx, READ, &data->simulation_ended))
	{
		i = -1;
		while (++i < data->philo_num && !read_change_bool(&data->mtx, READ, &data->simulation_ended))
		{
			if (gettime(MILLISECOND) - read_change_long(&data->philos[i].mtx, READ, &data->philos[i].last_meal_time) > (data->time_to_die / 1000))
			{
				write_message(&data->philos[i], DIE);
				read_change_bool(&data->mtx, CHANGE, &data->simulation_ended);
				return (NULL);
			}
		}
	}
	return (NULL);
}

int	start_dinner(t_data *data)
{
	int	i;

	i = -1;
	if (data->min_times_to_eat == 0)
		return (0);
/*	if (data->philo_num == 1)
		//TODO*/
	data->start = gettime(MILLISECOND);
	while (++i < data->philo_num)
		thread_action(&data->philos[i].philo_thread, routine, &data->philos[i], CREATE);
	read_change_bool(&data->mtx, CHANGE, &data->philos_ready);
	thread_action(&data->monitor_thread, monitor_thread, data, CREATE);
	i = -1;
	while (++i < data->philo_num)
		thread_action(&data->philos[i].philo_thread, NULL, NULL, JOIN);
	read_change_bool(&data->mtx, CHANGE, &data->simulation_ended);
	thread_action(&data->monitor_thread, NULL, NULL, JOIN);
	return (0);
}
