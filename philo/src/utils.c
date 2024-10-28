/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: discallow <discallow@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 15:09:06 by discallow         #+#    #+#             */
/*   Updated: 2024/10/22 17:18:25 by discallow        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_all_threads(t_data *data)
{
	while (1)
	{
		if (process_bool(&data->mtx, READ, &data->philos_ready))
			return ;
	}
}

int	thread_action(pthread_t *th, void *r(void *), void *data, t_code code)
{
	if (code == CREATE && pthread_create(th, NULL, r, data))
		return (printf(YELLOW"Error while creating thread."RESET"\n"));
	else if (code == JOIN && pthread_join(*th, NULL))
		return (printf(YELLOW"Error while joining thread."RESET"\n"));
	return (0);
}

int	mtx_actions(pthread_mutex_t *mtx, t_code code)
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

long	process_long(pthread_mutex_t *mtx, t_action action, long *value)
{
	long	check;

	check = 0;
	if (action == READ)
	{
		mtx_actions(mtx, LOCK);
		check = *value;
		mtx_actions(mtx, UNLOCK);
	}
	else if (action == CHANGE)
	{
		mtx_actions(mtx, LOCK);
		(*value)++;
		check = *value;
		mtx_actions(mtx, UNLOCK);
	}
	return (check);
}

bool	process_bool(pthread_mutex_t *mtx, t_action action, bool *value)
{
	bool	check;

	check = false;
	if (action == READ)
	{
		mtx_actions(mtx, LOCK);
		check = *value;
		mtx_actions(mtx, UNLOCK);
	}
	else if (action == CHANGE)
	{
		mtx_actions(mtx, LOCK);
		*value = true;
		check = value;
		mtx_actions(mtx, UNLOCK);
	}
	return (check);
}
