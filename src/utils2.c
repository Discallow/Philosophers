/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: discallow <discallow@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 15:09:06 by discallow         #+#    #+#             */
/*   Updated: 2024/10/18 13:51:41 by discallow        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


long	read_change_long(pthread_mutex_t *mtx, t_action action, long *value)
{
	long	check;

	if (action == READ)
	{
		mutex_actions(mtx, LOCK);
		check = *value;
		mutex_actions(mtx, UNLOCK);
	}
	else if (action == CHANGE)
	{
		mutex_actions(mtx, LOCK);
		(*value)++;
		check = *value;
		mutex_actions(mtx, UNLOCK);
	}
	return (check);
}

bool	read_change_bool(pthread_mutex_t *mtx, t_action action, bool *value)
{
	bool	check;

	if (action == READ)
	{
		mutex_actions(mtx, LOCK);
		check = *value;
		mutex_actions(mtx, UNLOCK);
	}
	else if (action == CHANGE)
	{
		mutex_actions(mtx, LOCK);
		*value = true;
		check = value;
		mutex_actions(mtx, UNLOCK);
	}
	return (check);
}
