/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 19:50:54 by abounab           #+#    #+#             */
/*   Updated: 2024/05/10 20:00:53 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/time.h>

typedef struct s_data{
	pthread_t	thread;
	int			id;
	long long			t_die;
	long long			t_eat;
	long long			t_sleep;
	int			*fork_available;
	int			fork_mine;
	int			*count_eat;
	int			*is_dead;
	
	long long			*program_timer;
	long long			timer;

	int			print;

	pthread_mutex_t	*fork_mutex_mine;
	pthread_mutex_t	*fork_mutex_other;
	pthread_mutex_t *mutex_died;
	pthread_mutex_t *mutex_timer;
	pthread_mutex_t *mutex_eat;
	pthread_mutex_t *mutex_printer;
}	t_data;

typedef struct s_philos{
	pthread_t	watcher;
	int	*all_eat;
	int	total_philos;
	int	dead;
	long long	time_begin;
	int	condition_eat;
	pthread_mutex_t	*mutex_died_parent;
	t_data *philos;
}	t_philos;


#define GREEN "\033[32m"

#define RED "\033[31m"

#define BLUE "\033[34m"

#define DEFAULT "\033[0m"

#endif