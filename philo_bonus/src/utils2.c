/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: discallow <discallow@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:57:01 by discallow         #+#    #+#             */
/*   Updated: 2024/10/25 03:19:44 by discallow        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	philo_died(t_data *data, int i)
{
	long	last_meal_time;

	mtx_actions(&data->philos[i].mtx, LOCK);
	last_meal_time = data->philos[i].last_meal_time;
	mtx_actions(&data->philos[i].mtx, UNLOCK);
	if (!process_bool(&data->sem, READ, &data->philos->philos_full)
		&& gettime(MILLISECOND) - last_meal_time > (data->time_to_die / 1000))
	{
		write_message(&data->philos[i], DIE);
		process_bool(&data->sem, CHANGE, &data->meal_end);
		sem_actions(&data->write_sem, WAIT, NULL, 0);
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
	while (!process_bool(&philo->data->sem, READ, &philo->data->meal_end))
	{
		if (philo_died(philo->data, index - 1))
		{
			while (++i < philo->data->philo_num)
				sem_actions(&philo->data->eat, POST, NULL, 0);
			return (NULL);
		}
	}
	return (NULL);
}

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
	if (start == LONG_MIN)
		return ;
	while (gettime(MICROSECOND) - start < microseconds)
	{
		if (process_bool(&data->sem, READ, &data->meal_end))
			return ;
		cur = gettime(MICROSECOND) - start;
		left = microseconds - cur;
		if (left > MILLISECOND)
			usleep(left / 2);
		else
		{
			while (1)
			{
				if ((gettime(MICROSECOND) - start >= microseconds))
					return ;
			}
		}
	}
}

void	write_message(t_philo *philo, t_message message)
{
	long	time;

	time = gettime(MILLISECOND) - philo->data->start;
	if (process_bool(&philo->data->sem, READ, &philo->data->meal_end))
		return ;
	sem_actions(&philo->data->write_sem, WAIT, NULL, 0);
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
	sem_actions(&philo->data->write_sem, POST, NULL, 0);
}
