/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 16:20:41 by abounab           #+#    #+#             */
/*   Updated: 2024/05/11 18:17:30 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <semaphore.h>
# include <sys/time.h>

typedef struct s_data{
	int			id;
	long long			t_die;
	long long			t_eat;
	long long			t_sleep;
	int			*count_eat;
	int			*is_dead;
	
	int			print;

	int			n_philos;
	
	long long			*program_timer;
	long long			timer;

	int			*forks;

	int			forks_mine;
	
	char	*str_printer;
	char	*str_timer;
	char	*str_eat;
	
	sem_t 	*sem_died;
	sem_t 	*sem_forks_checker;
	sem_t 	*sem_printer;//malloced ==>shared with main
	sem_t 	*sem_timer; //malloced ==>shared with main
	sem_t 	*sem_eat; //malloced ==>shared with main
}	t_data;

typedef struct s_philos{
	int	*all_eat;
	int	*arr_pid;
	int	total_philos;
	int	dead;
	int	forks_philos;
	long long	time_begin;
	int	condition_eat;
	sem_t	*sem_forks; //malloced ==> shared between processes
	sem_t	*sem_died_parent; //malloced ==>share with processes
	t_data *philos;
}	t_philos;


#define GREEN "\033[32m"

#define RED "\033[31m"

#define BLUE "\033[34m"

#define DEFAULT "\033[0m"



int	is_eating(t_data *philo);

#endif