/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sema_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 17:08:56 by abounab           #+#    #+#             */
/*   Updated: 2024/05/18 19:52:12 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	philo_is_died(t_data *philo)
{
	int	i;

	i = 0;
	sem_wait(philo->sem_died);
	if ((*philo->is_dead))
		i = philo->id;
	sem_post(philo->sem_died);
	return (i);
}

int	ft_printer(t_data *philo, char *str)
{
	if (!philo_is_died(philo))
		return (sem_wait(philo->sem_print), printf("%lld %d %s\n",
				(ft_get_utime() - (*philo->program_timer)), philo->id, str),
			sem_post(philo->sem_print), 1);
	return (0);
}

static int	create_watchers(t_philos *philo)
{
	int	i;

	i = 0;
	while (i < philo->total_philos)
	{
		pthread_create(&philo->philos[i].thread_philo, NULL,
			(void *)thread_routine, (void *)&philo->philos[i]);
		i++;
	}
	pthread_create(&philo->watcher, NULL, (void *)watcher_action,
		(void *)philo);
	sem_post(philo->sem_begin_all);
	return (1);
}

int	waiting_threads(t_philos philo)
{
	int	i;

	i = 0;
	create_watchers(&philo);
	while (i < philo.total_philos)
	{
		pthread_join(philo.philos[i].thread_philo, NULL);
		waitpid(philo.arr_pid[i], NULL, 0);
		i++;
	}
	pthread_join(philo.watcher, NULL);
	free_philos(&philo);
	return (1);
}
