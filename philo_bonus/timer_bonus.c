/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 17:27:00 by abounab           #+#    #+#             */
/*   Updated: 2024/05/17 18:49:55 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_usleep(long long timer)
{
	long long	val;

	val = ft_get_utime() * 1000;
	while (ft_get_utime() * 1000 < val + timer)
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

	sem_wait(philo->sem_timer_parent);
	now = ft_get_utime() - philo->timer;
	sem_post(philo->sem_timer_parent);
	if (now < philo->t_die)
		return (1);
	return (0);
}
