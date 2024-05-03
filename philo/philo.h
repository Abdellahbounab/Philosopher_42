/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 19:50:54 by abounab           #+#    #+#             */
/*   Updated: 2024/05/03 22:57:59 by abounab          ###   ########.fr       */
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
	int			*is_dead;
	int			count_eat;
	// pthread_mutex_t	fork_mutex_mine;
}	t_data;

typedef struct s_philos{
	int	total_philos;
	int	condition_eat;
	t_data *philos;
}	t_philos;


#endif