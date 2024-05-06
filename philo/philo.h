/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 19:50:54 by abounab           #+#    #+#             */
/*   Updated: 2024/05/06 21:00:29 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_data{
	pthread_t	thread;
	int			id;
	long long			t_die;
	long long			t_eat;
	long long			t_sleep;
	int			*fork_available;
	int			fork_mine;
	int			count_eat;
	int			*is_dead;
	long long			timer;
	pthread_mutex_t	*fork_mutex_mine;
	pthread_mutex_t	*fork_mutex_other;
	pthread_mutex_t	*mutex_died;
}	t_data;

typedef struct s_philos{
	int	total_philos;
	int	dead;
	long long	time_begin;
	int	condition_eat;
	t_data *philos;
}	t_philos;


#endif