/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:32:13 by abounab           #+#    #+#             */
/*   Updated: 2024/05/17 19:01:39 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	get_philos_time(t_philos *philo)
{
	int	i;

	i = 0;
	while (i < philo->total_philos)
	{
		philo->philos[i].timer = ft_get_utime();
		philo->philos[i].program_timer = &philo->time_begin;
		i++;
	}
	return (1);
}

void	ft_usleep(t_data *philo, long long timer)
{
	long long	val;

	val = ft_get_utime() * 1000;
	while (ft_get_utime() * 1000 < val + timer && check_printer(philo))
		usleep(70);
}

long long	ft_get_utime(void)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return (((now.tv_sec * 1000000) + now.tv_usec) / 1000);
}

int	compare_time(t_data *philo)
{
	long long	now;

	now = ft_get_utime();
	if (!philo_is_died(philo))
	{
		pthread_mutex_lock(philo->mutex_timer);
		if (now - philo->timer < philo->t_die)
			return (pthread_mutex_unlock(philo->mutex_timer), 1);
		return (pthread_mutex_unlock(philo->mutex_timer), 0);
	}
	return (-1);
}
