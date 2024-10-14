#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>
# include <limits.h>
# include <string.h>

# define RESET	"\033[0m"
# define RED	"\033[0;31m"
# define GREEN	"\033[0;32m"
# define YELLOW	"\033[0;33m"
# define CYAN	"\033[0;36m"

typedef struct s_data	t_data;

typedef enum	e_code
{
	INIT,
	DESTROY,
	LOCK,
	UNLOCK,
	CREATE,
	JOIN,
}				t_code;

typedef struct	s_fork
{
	pthread_mutex_t	mtx;
	long			fork_idx;

}				t_fork;

typedef struct	s_philo
{
	pthread_t		philo_thread;
	pthread_mutex_t	mtx;
	pthread_mutex_t	write_mtx;
	int				place_in_table;
	long			meals_num;
	t_fork			*first_fork;
	t_fork			*second_fork;
	bool			philos_ready;
	bool			philos_full;
	t_data			*data;
}				t_philo;

typedef	struct	s_data
{
	long	philo_num;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	min_times_to_eat;
	bool	simulation_ended;
	t_fork	*forks;
	t_philo	*philos;
}				t_data;

/* PARSING */
int	parse(t_data *data, char **av);

/* EXIT ERRORS */
int		invalid_args(void);
long	invalid_input(const char *s);

int	init_data(t_data *data);


#endif
