/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:34:07 by abounab           #+#    #+#             */
/*   Updated: 2024/05/17 19:01:27 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_sleeping(t_data *philo)
{
	if (!ft_printer(philo, "is sleeping"))
		return (0);
	if (philo->t_sleep + (ft_get_utime() - philo->timer) < philo->t_die)
	{
		ft_usleep(philo, philo->timer + philo->t_sleep * 1000 - ft_get_utime());
		if (!philo_is_died(philo))
			return (1);
		return (0);
	}
	return (0);
}

static int	is_thinking(t_data *philo, int cond)
{
	ft_printer(philo, "is thinking");
	if (!cond && !(philo->id % 2))
		ft_usleep(philo, philo->timer + (philo->t_eat * 1000) / 2
			- ft_get_utime());
	return (1);
}

static int	handle_fork_mine(t_data *philo)
{
	if (!philo_is_died(philo) && *philo->fork_available == philo->fork_mine)
	{
		pthread_mutex_unlock(philo->fork_mutex_mine);
		ft_usleep(philo, (philo->t_die - (ft_get_utime() - philo->timer))
			* 1000);
		return (is_dying(philo));
	}
	return (1);
}

static int	is_eating(t_data *philo)
{
	pthread_mutex_lock(philo->fork_mutex_mine);
	ft_printer(philo, "has taken a fork");
	if (handle_fork_mine(philo) && !philo_is_died(philo))
	{
		pthread_mutex_lock(philo->fork_mutex_other);
		if (ft_printer(philo, "has taken a fork") && ft_printer(philo,
				"is eating"))
		{
			pthread_mutex_lock(philo->mutex_timer);
			philo->timer = ft_get_utime();
			pthread_mutex_unlock(philo->mutex_timer);
			ft_usleep(philo, philo->timer + (philo->t_eat * 1000)
				- ft_get_utime());
			return (pthread_mutex_unlock(philo->fork_mutex_other),
				pthread_mutex_unlock(philo->fork_mutex_mine),
				1);
		}
		return (pthread_mutex_unlock(philo->fork_mutex_other),
			pthread_mutex_unlock(philo->fork_mutex_mine),
			0);
	}
	return (0);
}

int	turn_action(t_data *philo)
{
	is_thinking(philo, 0);
	while (!philo_is_died(philo) && *philo->count_eat)
	{
		if (is_eating(philo))
		{
			pthread_mutex_lock(philo->mutex_eat);
			if (*philo->count_eat > 0)
				(*philo->count_eat)--;
			pthread_mutex_unlock(philo->mutex_eat);
			if (is_sleeping(philo))
				is_thinking(philo, 1);
		}
	}
	return (1);
}
