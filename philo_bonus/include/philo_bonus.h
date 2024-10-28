/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: discallow <discallow@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:18:10 by discallow         #+#    #+#             */
/*   Updated: 2024/10/26 22:38:13 by discallow        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <pthread.h>
# include <stdbool.h>
# include <limits.h>
# include <string.h>
# include <semaphore.h>
# include <fcntl.h>
# include <signal.h>

# define RESET	"\033[0m"
# define RED	"\033[0;31m"
# define GREEN	"\033[0;32m"
# define YELLOW	"\033[0;33m"
# define CYAN	"\033[0;36m"

typedef struct s_data	t_data;

# define MICROSECOND 1000000
# define MILLISECOND 1000

# define MAX_PHILO_NAME "/max_philo_sem"
# define SEM_NAME "/main_sem"
# define WRITE_SEM_NAME "/write_sem"
# define FORKS_SEM_NAME "/forks_sem"
# define EAT_SEM_NAME "/eat_sem"
# define DEATH_SEM_NAME "/death_sem"

typedef enum e_action
{
	READ,
	CHANGE,
}				t_action;

typedef enum e_message
{
	FORK,
	EAT,
	THINK,
	SLEEP,
	DIE,
}				t_message;

typedef enum e_code
{
	INIT,
	DESTROY,
	LOCK,
	UNLOCK,
	CREATE,
	JOIN,
	DETACH,
	OPEN,
	CLOSE,
	UNLINK,
	POST,
	WAIT,
}				t_code;

typedef struct s_fork
{
	sem_t			*sem;
	long			fork_idx;
}					t_fork;

typedef struct s_philo
{
	pid_t			pid;
	pthread_mutex_t	mtx;
	int				place_in_table;
	long			meal_counter;
	bool			philos_full;
	long			last_meal_time;
	t_data			*data;
	pthread_t		monitor_thread;
}					t_philo;

typedef struct s_data
{
	sem_t			*sem_data;
	sem_t			*sem_write;
	sem_t			*sem_max_philo;
	sem_t			*sem_eat;
	sem_t			*sem_death;
	pthread_t		kill_philos;
	pthread_t		check_phillo_full;
	long			philo_num;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			min_times_to_eat;
	long			start;
	t_fork			*forks;
	t_philo			*philos;
}					t_data;

/* PARSING */
int		parse(t_data *data, char **av);

/* EXIT ERRORS */
int		invalid_args(void);
long	invalid_input(const char *s);

int		init_data(t_data *data);

/* MUTEX AND THREADS UTILS */
int		thread_action(pthread_t *th, void *r(void *), void *data, t_code code);
int		sem_actions(sem_t **sem, t_code code, char *name, unsigned int value);
int		mtx_actions(pthread_mutex_t *mtx, t_code code);
long	process_long(sem_t **sem, t_action action, long *value);
bool	process_bool(sem_t **sem, t_action action, bool *value);

/*UTILS */
long	gettime(long time);
void	improved_usleep(long microseconds);
void	write_message(t_philo *philo, t_message message);
void	clean_data(t_data *data);

/*MEALS FUNCTIONS*/
int		start_dinner(t_data *data);
void	*monitor_thread(void *arg);

#endif
