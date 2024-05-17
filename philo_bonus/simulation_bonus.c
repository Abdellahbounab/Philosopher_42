/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 17:06:59 by abounab           #+#    #+#             */
/*   Updated: 2024/05/17 18:55:06 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	is_sleeping(t_data *philo)
{
	if (!ft_printer(philo, "is sleeping"))
		return (0);
	if (philo->t_sleep + (ft_get_utime() - philo->timer) < philo->t_die)
	{
		ft_usleep(philo->timer + philo->t_sleep * 1000 - ft_get_utime());
		return (1);
	}
	return (0);
}

static int	is_thinking(t_data *philo, int cond)
{
	ft_printer(philo, "is thinking");
	if (!cond && !(philo->id % 2))
		ft_usleep(philo->t_eat / 2 * 1000);
	return (1);
}

static int	handle_fork_mine(t_data *philo)
{
	sem_wait(philo->sem_forks_checker);
	ft_printer(philo, "has taken a fork");
	if (philo->n_philos == 1)
		return (ft_usleep((philo->t_die - (ft_get_utime() - philo->timer))
				* 1000), is_dying(philo));
	else
		return (1);
}

static int	is_eating(t_data *philo)
{
	if (handle_fork_mine(philo))
	{
		sem_wait(philo->sem_forks_checker);
		ft_printer(philo, "has taken a fork");
		ft_printer(philo, "is eating");
		sem_post(philo->sem_timer);
		philo->timer = ft_get_utime();
		ft_usleep(philo->timer + (philo->t_eat * 1000) - ft_get_utime());
		sem_post(philo->sem_forks_checker);
		sem_post(philo->sem_forks_checker);
		return (1);
	}
	return (0);
}

int	turn_action(t_data *philo)
{
	get_program_timer(philo);
	is_thinking(philo, 0);
	while (philo->count_eat)
	{
		if (is_eating(philo))
		{
			if (philo->count_eat > 0)
				(philo->count_eat)--;
			if (is_sleeping(philo))
				is_thinking(philo, 1);
		}
	}
	return (1);
}
