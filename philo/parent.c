/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:30:55 by abounab           #+#    #+#             */
/*   Updated: 2024/05/17 19:00:53 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_dying(t_data *philo)
{
	if (!philo_is_died(philo))
	{
		pthread_mutex_lock(philo->mutex_died);
		if (*philo->count_eat)
			*philo->is_dead = philo->id;
		pthread_mutex_unlock(philo->mutex_died);
		if (*philo->count_eat)
			printf("%s%lld %d died%s\n", RED, ((ft_get_utime()
						- *philo->program_timer)), philo->id, DEFAULT);
	}
	return (0);
}

static int	annonce_death(t_philos *philo)
{
	pthread_mutex_lock(philo->mutex_print_parent);
	*philo->print_parent = 0;
	pthread_mutex_unlock(philo->mutex_print_parent);
	return (1);
}

int	watcher_action(t_philos *philo)
{
	int	i;

	i = 0;
	while (!philo->dead)
	{
		pthread_mutex_lock(philo->philos[i].mutex_eat);
		if (philo->philos[i].count_eat)
		{
			pthread_mutex_unlock(philo->philos[i].mutex_eat);
			if (!compare_time(&philo->philos[i]))
				return (annonce_death(philo), is_dying(&philo->philos[i]));
		}
		else
			pthread_mutex_unlock(philo->philos[i].mutex_eat);
		if (i < philo->total_philos - 1)
			i++;
		else
			i = 0;
	}
	return (1);
}
