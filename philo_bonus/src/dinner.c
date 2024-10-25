/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: discallow <discallow@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 12:33:33 by discallow         #+#    #+#             */
/*   Updated: 2024/10/25 04:02:18 by discallow        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	eat(t_philo *philo)
{
	sem_actions(&philo->data->max_philo, WAIT, NULL, 0);
	sem_actions(&philo->data->forks->sem, WAIT, NULL, 0);
	write_message(philo, FORK);
	if (philo->data->philo_num == 1)
	{
		while (!process_bool(&philo->data->sem, READ,
				& philo->data->meal_end))
			usleep(200);
		sem_actions(&philo->data->forks->sem, POST, NULL, 0);
		return ;
	}
	sem_actions(&philo->data->forks->sem, WAIT, NULL, 0);
	write_message(philo, FORK);
	write_message(philo, EAT);
	mtx_actions(&philo->mtx, LOCK);
	philo->last_meal_time = gettime(MILLISECOND);
	mtx_actions(&philo->mtx, UNLOCK);
	philo->meal_counter++;
	improved_usleep(philo->data->time_to_eat, philo->data);
	if (philo->data->min_times_to_eat > 0
		&& philo->data->min_times_to_eat == philo->meal_counter)
		sem_actions(&philo->data->eat, POST, NULL, 0);
	sem_actions(&philo->data->forks->sem, POST, NULL, 0);
	sem_actions(&philo->data->forks->sem, POST, NULL, 0);
	sem_actions(&philo->data->max_philo, POST, NULL, 0);
}

void	think(t_philo *philo, int *flag)
{
	if (process_bool(&philo->data->sem, READ, &philo->data->meal_end))
		return ;
	if (*flag)
		*flag = 0;
	else
		write_message(philo, THINK);
	if (philo->data->philo_num % 2 == 0)
		return ;
	if (philo->place_in_table % 2)
		improved_usleep((philo->data->time_to_eat * 2
				- philo->data->time_to_sleep) * 0.3, philo->data);
}

void	routine(t_philo *philo)
{
	t_data	*data;
	int		flag;

	data = philo->data;
	flag = 1;
	mtx_actions(&philo->mtx, LOCK);
	philo->last_meal_time = gettime(MILLISECOND);
	mtx_actions(&philo->mtx, UNLOCK);
	think(philo, &flag);
	while (1)
	{
		if (process_bool(&data->sem, READ, &data->meal_end)
			|| process_bool(&data->sem, READ, &data->philos->philos_full))
			break ;
		eat(philo);
		write_message(philo, SLEEP);
		improved_usleep(philo->data->time_to_sleep, philo->data);
		think(philo, &flag);
	}
	exit (EXIT_SUCCESS);
}

void	*kill_func(void *philo)
{
	t_philo	*philos;
	int		i;

	philos = (t_philo *)philo;
	sem_wait(philos->data->sem_death);
/*	if (philos->info->times_must_eat > -1)
		usleep(1000);*/
	i = -1;
	while (++i < philos->data->philo_num)
		kill(philos[i].pid, SIGKILL);
	return (NULL);
}

void	*meals_func(void *arg)
{
	t_philo	*philos;
	int		full;

	philos = (t_philo *)arg;
	full = -1;
	while (++full < philos->data->philo_num)
		sem_wait(philos->data->eat);
	sem_post(philos->data->sem_death);
	return (NULL);
}

void	checking_threads(t_data *data)
{
	if (data->min_times_to_eat > -1)
	{
		pthread_create(&data->verify_satisfied, NULL, meals_func,
			data->philos);
		pthread_detach(data->verify_satisfied);
	}
	pthread_create(&data->kill_philos, NULL, kill_func, data->philos);
	pthread_detach(data->kill_philos);
}

int	start_dinner(t_data *data)
{
	int	i;

	i = -1;
	if (data->min_times_to_eat == 0)
		return (0);
	data->start = gettime(MILLISECOND);
	while (++i < data->philo_num)
	{
		data->philos[i].pid = fork();
		if (data->philos[i].pid < 0)
		{
			printf(YELLOW"Error while forking"RESET"\n");
			exit (1);
		}
		if (data->philos[i].pid == 0)
		{
			pthread_create(&data->philos[i].monitor_thread, NULL, monitor_thread, &data->philos[i]);
			pthread_detach(data->philos[i].monitor_thread);
			routine(data->philos + i);
		}
	}
	checking_threads(data);
	waitpid(-1, NULL, 0);
	return (0);
}
