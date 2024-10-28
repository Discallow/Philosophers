/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: discallow <discallow@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 16:57:01 by discallow         #+#    #+#             */
/*   Updated: 2024/10/26 22:43:26 by discallow        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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

void	improved_usleep(long microseconds)
{
	long	start;
	long	cur;
	long	left;

	start = gettime(MICROSECOND);
	if (start == LONG_MIN)
		return ;
	while (gettime(MICROSECOND) - start < microseconds)
	{
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
	sem_actions(&philo->data->sem_write, WAIT, NULL, 0);
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
	sem_actions(&philo->data->sem_write, POST, NULL, 0);
}
