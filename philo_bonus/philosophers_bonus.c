/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 17:08:31 by abounab           #+#    #+#             */
/*   Updated: 2024/05/17 18:59:30 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	get_philo_args(t_philos *philo, int *condition, int ids, char **av)
{
	char	*id_str;

	id_str = ft_itoa(ids);
	philo->philos[ids - 1].id = ids;
	philo->philos[ids - 1].t_die = ft_atoi(av[0]);
	philo->philos[ids - 1].t_eat = ft_atoi(av[1]);
	philo->philos[ids - 1].t_sleep = ft_atoi(av[2]);
	philo->philos[ids - 1].n_philos = philo->total_philos;
	philo->philos[ids - 1].is_dead = &philo->dead;
	philo->philos[ids - 1].sem_forks_checker = philo->sem_forks;
	philo->philos[ids - 1].sem_print = philo->sem_printer_parent;
	philo->philos[ids - 1].sem_died = philo->sem_died_parent;
	philo->philos[ids - 1].sem_begin = philo->sem_begin_all;
	philo->philos[ids - 1].str_child = ft_strjoin("/sem_timer_child", id_str);
	philo->philos[ids - 1].str_parent = ft_strjoin("/sem_timer_parent", id_str);
	sem_unlink(philo->philos[ids - 1].str_parent);
	sem_unlink(philo->philos[ids - 1].str_child);
	philo->philos[ids - 1].sem_timer_parent = sem_open(philo->philos[ids
			- 1].str_parent, O_CREAT, 0640, 1);
	philo->philos[ids - 1].sem_timer = sem_open(philo->philos[ids
			- 1].str_child, O_CREAT, 0640, 0);
	free(id_str);
	if (av[3])
		*condition = ft_atoi(av[3]);
	return (philo->philos[ids - 1].count_eat = *condition, 1);
}

int	free_philos(t_philos *philo)
{
	int	i;

	i = 0;
	while (i < philo->total_philos)
	{
		sem_close(philo->philos[i].sem_timer);
		sem_close(philo->philos[i].sem_timer_parent);
		sem_unlink(philo->philos[i].str_parent);
		sem_unlink(philo->philos[i].str_child);
		free(philo->philos[i].str_child);
		free(philo->philos[i].str_parent);
		i++;
	}
	sem_close(philo->sem_died_parent);
	sem_close(philo->sem_forks);
	sem_close(philo->sem_begin_all);
	free(philo->arr_pid);
	free(philo->philos);
	return (1);
}
