/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: discallow <discallow@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 14:13:06 by discallow         #+#    #+#             */
/*   Updated: 2024/10/22 17:16:58 by discallow        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*valgrind --tool=hellgrind*/
#include "philo.h"

void	clean_data(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_num)
	{
		mtx_actions(&data->forks[i].mtx, DESTROY);
		mtx_actions(&data->philos[i].mtx, DESTROY);
	}
	mtx_actions(&data->mtx, DESTROY);
	mtx_actions(&data->write_mtx, DESTROY);
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
