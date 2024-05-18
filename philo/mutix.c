/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutix.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:35:36 by abounab           #+#    #+#             */
/*   Updated: 2024/05/18 19:58:28 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_is_died(t_data *philo)
{
	int	i;

	i = 0;
	pthread_mutex_lock(philo->mutex_died);
	if ((*philo->is_dead))
		i = philo->id;
	pthread_mutex_unlock(philo->mutex_died);
	return (i);
}

int	get_lock(t_data *philo)
{
	int	i;

	i = 0;
	pthread_mutex_lock(philo->mutex_printer);
	if (*philo->print)
		i = 1;
	pthread_mutex_unlock(philo->mutex_printer);
	return (i);
}

int	ft_printer(t_data *philo, char *str)
{
	pthread_mutex_lock(philo->mutex_printer);
	if (*philo->print)
		printf("%lld %d %s\n", ((ft_get_utime() - *philo->program_timer)),
			philo->id, str);
	else
		return (pthread_mutex_unlock(philo->mutex_printer), 0);
	pthread_mutex_unlock(philo->mutex_printer);
	return (1);
}

int	get_other_mutex(t_philos *philo)
{
	int	i;

	i = 0;
	while (i < philo->total_philos - 1)
	{
		philo->philos[i].fork_mutex_other = philo->philos[i
			+ 1].fork_mutex_mine;
		i++;
	}
	philo->philos[i].fork_mutex_other = philo->philos[0].fork_mutex_mine;
	return (1);
}

int	free_philos(t_philos *philo)
{
	int	i;

	i = 0;
	while (i < philo->total_philos)
	{
		pthread_mutex_destroy(philo->philos[i].fork_mutex_mine);
		free(philo->philos[i].fork_mutex_mine);
		pthread_mutex_destroy(philo->philos[i].mutex_timer);
		free(philo->philos[i].mutex_timer);
		pthread_mutex_destroy(philo->philos[i].mutex_eat);
		free(philo->philos[i].mutex_eat);
		i++;
	}
	pthread_mutex_destroy(philo->mutex_died_parent);
	pthread_mutex_destroy(philo->mutex_print_parent);
	free(philo->mutex_print_parent);
	free(philo->mutex_died_parent);
	free(philo->philos);
	return (1);
}
