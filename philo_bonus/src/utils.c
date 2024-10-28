/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: discallow <discallow@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 15:09:06 by discallow         #+#    #+#             */
/*   Updated: 2024/10/26 22:23:45 by discallow        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	mtx_actions(pthread_mutex_t *mtx, t_code code)
{
	if (code == INIT && pthread_mutex_init(mtx, NULL))
		exit (printf(YELLOW"Error while starting mutex."RESET"\n"));
	else if (code == DESTROY && pthread_mutex_destroy(mtx))
		exit (printf(YELLOW"Error while destroying mutex."RESET"\n"));
	else if (code == LOCK && pthread_mutex_lock(mtx))
		exit (printf(YELLOW"Error while locking mutex."RESET"\n"));
	else if (code == UNLOCK && pthread_mutex_unlock(mtx))
		exit (printf(YELLOW"Error while unlocking mutex."RESET"\n"));
	return (0);
}

int	thread_action(pthread_t *th, void *r(void *), void *data, t_code code)
{
	if (code == CREATE && pthread_create(th, NULL, r, data))
		return (printf(YELLOW"Error while creating thread."RESET"\n"));
	else if (code == JOIN && pthread_join(*th, NULL))
		return (printf(YELLOW"Error while joining thread."RESET"\n"));
	else if (code == DETACH && pthread_detach(*th))
		return (printf(YELLOW"Error while detaching thread."RESET"\n"));
	return (0);
}

int	sem_actions(sem_t **sem, t_code code, char *name, unsigned int value)
{
	if (code == OPEN)
	{
		*sem = sem_open(name, O_CREAT | O_EXCL, 0666, value);
		if (*sem == SEM_FAILED)
			return (printf(YELLOW"Error while opening semaphore."RESET"\n"));
	}
	else if (code == CLOSE)
	{
		if (sem_close(*sem))
			return (printf(YELLOW"Error while closing semaphore."RESET"\n"));
	}
	else if (code == POST && sem_post(*sem))
		return (printf(YELLOW"Error while unlocking semaphore."RESET"\n"));
	else if (code == WAIT && sem_wait(*sem))
		return (printf(YELLOW"Error while locking semaphore."RESET"\n"));
	return (0);
}

long	process_long(sem_t **sem, t_action action, long *value)
{
	long	check;

	check = 0;
	if (action == READ)
	{
		sem_wait(*sem);
		check = *value;
		sem_post(*sem);
	}
	else if (action == CHANGE)
	{
		sem_wait(*sem);
		(*value)++;
		check = *value;
		sem_post(*sem);
	}
	return (check);
}

bool	process_bool(sem_t **sem, t_action action, bool *value)
{
	bool	check;

	check = false;
	if (action == READ)
	{
		sem_wait(*sem);
		check = *value;
		sem_post(*sem);
	}
	else if (action == CHANGE)
	{
		sem_wait(*sem);
		*value = true;
		check = value;
		sem_post(*sem);
	}
	return (check);
}
