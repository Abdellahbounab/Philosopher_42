/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 19:50:33 by abounab           #+#    #+#             */
/*   Updated: 2024/05/17 19:00:19 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	waiting_threads(t_philos philo)
{
	int	i;

	i = 0;
	while (i < philo.total_philos)
	{
		pthread_join(philo.philos[i].thread, NULL);
		i++;
	}
	if (!philo_is_died(&philo.philos[i - 1]))
		is_dying(&philo.philos[i - 1]);
	pthread_join(philo.watcher, NULL);
	free_philos(&philo);
	return (1);
}

static int	run_simulation(t_philos philo)
{
	int	i;

	i = 0;
	get_philos_time(&philo);
	while (i < philo.total_philos)
	{
		pthread_create(&philo.philos[i].thread, NULL, (void *)turn_action,
			(void *)&philo.philos[i]);
		i++;
	}
	pthread_create(&philo.watcher, NULL, (void *)watcher_action,
		(void *)&philo);
	waiting_threads(philo);
	return (1);
}

int	main(int ac, char **av)
{
	t_philos	philo;

	if (ac >= 5 && ac <= 6)
	{
		if (get_philos_data(&philo, av + 1))
			run_simulation(philo);
	}
}
