/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 19:50:54 by abounab           #+#    #+#             */
/*   Updated: 2024/05/04 20:52:23 by abounab          ###   ########.fr       */
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
	int			t_die;
	int			t_eat;
	int			t_sleep;
	int			*fork_available;
	int			fork_mine;
	int			count_eat;
	int			*is_dead;
	struct timeval *start_time;
	pthread_mutex_t	*fork_mutex_mine;
	pthread_mutex_t	*fork_mutex_other;
}	t_data;

typedef struct s_philos{
	int	total_philos;
	int	dead;
	struct timeval time_begin;
	int	condition_eat;
	t_data *philos;
}	t_philos;


#endif