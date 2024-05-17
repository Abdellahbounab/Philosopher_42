/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 16:20:41 by abounab           #+#    #+#             */
/*   Updated: 2024/05/17 18:54:08 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data
{
	int			id;
	long long	t_die;
	long long	t_eat;
	long long	t_sleep;

	int			count_eat;
	int			*is_dead;

	int			n_philos;

	long long	program_timer;
	long long	timer;

	char		*str_child;
	char		*str_parent;

	pthread_t	thread_philo;

	sem_t		*sem_died;
	sem_t		*sem_forks_checker;
	sem_t		*sem_begin;
	sem_t		*sem_print;
	sem_t		*sem_timer;
	sem_t		*sem_timer_parent;
}				t_data;

typedef struct s_philos
{
	int			*arr_pid;
	int			total_philos;
	int			dead;
	long long	time_begin;
	int			condition_eat;

	pthread_t	watcher;

	sem_t		*sem_begin_all;
	sem_t		*sem_died_parent;
	sem_t		*sem_forks;
	sem_t		*sem_printer_parent;
	t_data		*philos;
}				t_philos;

# define GREEN "\033[32m"

# define RED "\033[31m"

# define BLUE "\033[34m"

# define DEFAULT "\033[0m"

int				get_philo_args(t_philos *philo, int *condition, int ids,
					char **av);
int				free_philos(t_philos *philo);

int				philo_is_died(t_data *philo);
int				ft_printer(t_data *philo, char *str);
int				waiting_threads(t_philos philo);

int				turn_action(t_data *philo);

int				is_dying(t_data *philo);
int				get_program_timer(t_data *philo);
int				thread_routine(t_data *philo);
int				watcher_action(t_philos *philo);

void			ft_usleep(long long timer);
long long		ft_get_utime(void);
int				compare_time(t_data *philo);

char			*ft_itoa(int n);
int				ft_atoi(char *str);
char			*ft_strjoin(char const *s1, char const *s2);

#endif