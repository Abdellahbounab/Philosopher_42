/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 17:26:16 by abounab           #+#    #+#             */
/*   Updated: 2024/05/17 18:59:17 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	get_program_timer(t_data *philo)
{
	sem_wait(philo->sem_begin);
	philo->timer = ft_get_utime();
	philo->program_timer = ft_get_utime();
	sem_post(philo->sem_begin);
	return (1);
}

int	is_dying(t_data *philo)
{
	sem_wait(philo->sem_print);
	sem_wait(philo->sem_died);
	if (philo->count_eat)
	{
		*philo->is_dead = philo->id;
		printf("%s%lld %d died%s\n", RED, ((ft_get_utime()
					- philo->program_timer)), philo->id, DEFAULT);
	}
	return (1);
}

int	thread_routine(t_data *philo)
{
	sem_wait(philo->sem_timer_parent);
	philo->timer = ft_get_utime();
	sem_post(philo->sem_timer_parent);
	philo->program_timer = ft_get_utime();
	while (philo->count_eat)
	{
		sem_wait(philo->sem_timer);
		philo->timer = ft_get_utime();
		sem_wait(philo->sem_timer_parent);
		if (philo->count_eat > 0)
			philo->count_eat--;
		sem_post(philo->sem_timer_parent);
		if (*philo->is_dead)
			return (0);
	}
	return (1);
}

static int	annonce_death(t_philos *philo, int pos)
{
	int	i;

	i = 0;
	is_dying(&philo->philos[pos]);
	while (i < philo->total_philos)
	{
		kill(philo->arr_pid[i], SIGKILL);
		sem_post(philo->philos[i].sem_timer);
		sem_post(philo->philos[i].sem_timer_parent);
		i++;
	}
	return (1);
}

int	watcher_action(t_philos *philo)
{
	int	i;

	i = 0;
	while (!philo->dead)
	{
		usleep(500);
		sem_wait(philo->philos[i].sem_timer_parent);
		if (philo->philos[i].count_eat)
		{
			sem_post(philo->philos[i].sem_timer_parent);
			if (!compare_time(&philo->philos[i]))
				return (annonce_death(philo, i));
		}
		else
			return (sem_post(philo->philos[i].sem_timer_parent), 1);
		if (i < philo->total_philos - 1)
			i++;
		else
			i = 0;
	}
	return (1);
}
