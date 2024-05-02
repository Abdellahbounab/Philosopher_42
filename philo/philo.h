/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 19:50:54 by abounab           #+#    #+#             */
/*   Updated: 2024/05/02 19:48:44 by abounab          ###   ########.fr       */
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
	int			fork_available;
	int			t_sleep;
	int			count_eat;
}	t_data;

typedef struct s_philos{
	int	total_philos;
	int	condition_eat;
	t_data *philos;
}	t_philos;


#endif