/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: discallow <discallow@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:33:33 by discallow         #+#    #+#             */
/*   Updated: 2024/10/15 20:44:24 by discallow        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	gettime(void)
{
	struct timeval	start;
	long			elapsed;

	if (gettimeofday(&start, NULL))
	{
		printf(YELLOW"gettimeofday function failed."RESET"\n");
		return (LONG_MIN);
	}
	elapsed = (start.tv_sec * MICROSECOND) + start.tv_usec;
	return (elapsed);
}
void	improved_usleep(long microseconds, t_data *data)
{
	long	start;
	long	cur;
	long	left;

	start = gettime();
	while (gettime() - start < microseconds)
	{
		mutex_actions(&data->mtx, LOCK);
		if (data->simulation_ended)
		{
			mutex_actions(&data->mtx, UNLOCK);
			return ;
		}
		mutex_actions(&data->mtx, UNLOCK);
		cur = gettime() - start;
		left = microseconds - cur;
		if (left == MILLISECOND)
			usleep(left / 2);
		else
		{
			while (1)
				if (gettime() - start >= microseconds)
					break ;
		}
	}
}

void	wait_all_threads(t_data *data)
{
	while (1)
	{
		mutex_actions(&data->mtx, LOCK);
		if (data->philos_ready == true)
		{
			mutex_actions(&data->mtx, UNLOCK);
			break ;
		}
		mutex_actions(&data->mtx, UNLOCK);
	}
}

void	write_message(t_philo *philo, t_message message)
{
	long	time;

	mutex_actions(&philo->data->mtx, LOCK);
	time = gettime() - philo->data->start;
	mutex_actions(&philo->data->mtx, UNLOCK);
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
	mutex_actions(&philo->data->mtx, LOCK);
	if (philo->data->simulation_ended || philo->data->philos_full)
	{
		mutex_actions(&philo->data->mtx, UNLOCK);
		return ;
	}
	mutex_actions(&philo->data->mtx, UNLOCK);
	mutex_actions(&philo->first_fork->mtx, LOCK);
	write_message(philo, FORK);
	mutex_actions(&philo->second_fork->mtx, LOCK);
	write_message(philo, FORK);
	write_message(philo, EAT);
	improved_usleep(MICROSECOND, philo->data);
	mutex_actions(&philo->mtx, LOCK);
	philo->meal_counter++;
	mutex_actions(&philo->mtx, UNLOCK);
	if (philo->data->min_times_to_eat > 0
		&& philo->data->min_times_to_eat == philo->meal_counter)
	{
		mutex_actions(&philo->mtx, LOCK);
		philo->data->num_philos_full++;
		if (philo->data->num_philos_full == philo->data->philo_num)
			philo->data->philos_full = true;
		mutex_actions(&philo->mtx, UNLOCK);
	}
	mutex_actions(&philo->first_fork->mtx, UNLOCK);
	mutex_actions(&philo->second_fork->mtx, UNLOCK);
}

void	think(t_philo *philo)
{
	mutex_actions(&philo->data->mtx, LOCK);
	if (philo->data->simulation_ended)
	{
		mutex_actions(&philo->data->mtx, UNLOCK);
		return ;
	}
	mutex_actions(&philo->data->mtx, UNLOCK);
	write_message(philo, THINK);
}
void	*routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	wait_all_threads(data);
	while (1)
	{
		mutex_actions(&data->mtx, LOCK);
		if (data->simulation_ended || philo->data->philos_full)
		{
			mutex_actions(&data->mtx, UNLOCK);
			return (NULL);
		}
		mutex_actions(&data->mtx, UNLOCK);
		eat(philo);
		mutex_actions(&philo->mtx, LOCK);
		write_message(philo, SLEEP);
		improved_usleep(philo->data->time_to_sleep, philo->data);
		mutex_actions(&philo->mtx, UNLOCK);
		think(philo);
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
	data->start = gettime();
	while (++i < data->philo_num)
		thread_action(&data->philos[i].philo_thread, routine, &data->philos[i], CREATE);
	mutex_actions(&data->mtx, LOCK);
	data->philos_ready = true;
	mutex_actions(&data->mtx, UNLOCK);
	i = -1;
	while (++i < data->philo_num)
		thread_action(&data->philos[i].philo_thread, NULL, NULL, JOIN);
	return (0);
}
