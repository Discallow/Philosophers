/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: discallow <discallow@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 14:13:06 by discallow         #+#    #+#             */
/*   Updated: 2024/10/26 22:10:23 by discallow        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	clean_data(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_num)
		mtx_actions(&data->philos[i].mtx, DESTROY);
	sem_actions(&data->sem_data, CLOSE, NULL, 0);
	sem_actions(&data->sem_write, CLOSE, NULL, 0);
	sem_actions(&data->forks->sem, CLOSE, NULL, 0);
	sem_actions(&data->sem_max_philo, CLOSE, NULL, 0);
	sem_actions(&data->sem_eat, CLOSE, NULL, 0);
	sem_actions(&data->sem_death, CLOSE, NULL, 0);
	i = -1;
	while (++i < data->philo_num && data->min_times_to_eat > 0)
	{
		if (data->philos[i].pid != -1)
			kill(data->philos[i].pid, SIGKILL);
	}
	free(data->forks);
	free(data->philos);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
		return (invalid_args());
	if (parse(&data, argv) || init_data(&data))
		return (1);
	start_dinner(&data);
	clean_data(&data);
	return (0);
}
