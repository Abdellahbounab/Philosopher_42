/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 12:37:21 by abounab           #+#    #+#             */
/*   Updated: 2024/05/17 18:55:38 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	param_is_correct(char **av)
{
	if (ft_atoi(av[1]) > 0 && ft_atoi(av[2]) > 0 && ft_atoi(av[3]) > 0)
	{
		if (av[4] && ft_atoi(av[4]) < 0)
			return (0);
		return (1);
	}
	return (0);
}

static int	get_philos_data(t_philos *philo, char **av)
{
	int	i;

	i = 0;
	philo->total_philos = ft_atoi(av[0]);
	if (philo->total_philos <= 0)
		return (0);
	if (!param_is_correct(av))
		return (0);
	philo->philos = malloc(sizeof(t_data) * philo->total_philos);
	philo->dead = 0;
	philo->arr_pid = malloc(sizeof(int) * philo->total_philos);
	sem_unlink("/sem_died");
	sem_unlink("/sem_forks_all");
	sem_unlink("/sem_sem_begin_all");
	sem_unlink("/sem_printer");
	philo->sem_died_parent = sem_open("/sem_died", O_CREAT, 0640, 1);
	philo->sem_begin_all = sem_open("/sem_sem_begin_all", O_CREAT, 0640, 0);
	philo->sem_printer_parent = sem_open("/sem_printer", O_CREAT, 0640, 1);
	philo->sem_forks = sem_open("/sem_forks_all", O_CREAT, 0640,
			philo->total_philos);
	philo->condition_eat = -1;
	while (i < philo->total_philos)
		get_philo_args(philo, &philo->condition_eat, ++i, av + 1);
	return (1);
}

static int	run_simulation(t_philos philo)
{
	int	i;
	int	pid_cpy;

	i = 0;
	pid_cpy = 0;
	while (i < philo.total_philos)
	{
		pid_cpy = fork();
		if (!pid_cpy)
			return (turn_action(&philo.philos[i]));
		else
			philo.arr_pid[i] = pid_cpy;
		i++;
	}
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
