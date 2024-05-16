/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 16:20:41 by abounab           #+#    #+#             */
/*   Updated: 2024/05/16 12:44:03 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <semaphore.h>
# include <sys/time.h>

typedef struct s_data{
	int			id;
	long long			t_die;
	long long			t_eat;
	long long			t_sleep;
	
	int			count_eat;
	int			*is_dead;

	int			n_philos;
	
	long long			program_timer;
	long long			timer;
	
	char	*str_child;
	char	*str_parent;
	
	pthread_t	thread_philo;
	
	sem_t 	*sem_died;
	sem_t 	*sem_forks_checker;
	sem_t 	*sem_begin;
	sem_t 	*sem_print;
	sem_t 	*sem_timer; //malloced ==>shared with main
	sem_t 	*sem_timer_parent; //malloced ==>shared with main
}	t_data;

typedef struct s_philos{
	int	*arr_pid;
	int	total_philos;
	int	dead;
	// int	forks_philos;
	long long	time_begin;
	int	condition_eat;

	pthread_t watcher;

	sem_t 	*sem_begin_all;//malloced ==>
	sem_t 	*sem_died_parent; //malloced
	sem_t	*sem_forks; //malloced ==> shared between processes
	sem_t	*sem_printer_parent; //malloced ==> shared between processes
	t_data *philos;
}	t_philos;


#define GREEN "\033[32m"

#define RED "\033[31m"

#define BLUE "\033[34m"

#define DEFAULT "\033[0m"



int	is_eating(t_data *philo);

#endif