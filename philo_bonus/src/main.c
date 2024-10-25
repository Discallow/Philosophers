/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: discallow <discallow@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 14:13:06 by discallow         #+#    #+#             */
/*   Updated: 2024/10/25 03:59:22 by discallow        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	clean_data(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_num)
		mtx_actions(&data->philos[i].mtx, DESTROY);
	if (sem_actions(&data->sem, CLOSE, NULL, 0)
		|| sem_actions(&data->write_sem, CLOSE, NULL, 0)
			|| sem_actions(&data->forks->sem, CLOSE, NULL, 0)
			|| sem_actions(&data->max_philo, CLOSE, NULL, 0)
			|| sem_actions(&data->eat, CLOSE, NULL, 0)
			|| sem_actions(&data->sem_death, CLOSE, NULL, 0))
		exit(1);
	i = -1;
	while (++i < data->philo_num)
	{
		if (data->philos[i].pid != -1)
			kill(data->philos[i].pid, SIGKILL);
	}
	if (sem_actions(&data->sem, UNLINK, data->sem_name, 0)
		|| sem_actions(&data->write_sem, UNLINK, data->write_sem_name, 0)
			|| sem_actions(&data->forks->sem, UNLINK, data->forks->sem_name, 0)
			|| sem_actions(&data->max_philo, UNLINK, data->max_philo_name, 0)
			|| sem_actions(&data->eat, UNLINK, data->eat_sem_name, 0)
			|| sem_actions(&data->sem_death, UNLINK, data->sem_death_name, 0))
		exit (1);
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
