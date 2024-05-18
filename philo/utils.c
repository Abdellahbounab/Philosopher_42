/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 14:38:29 by abounab           #+#    #+#             */
/*   Updated: 2024/05/18 19:57:09 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_atoi(char *str)
{
	int	num;
	int	sign;

	num = 0;
	sign = 1;
	if (str && (*str == '-' || *str == '+'))
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (str && *str && *str >= '0' && *str <= '9')
	{
		num *= 10;
		num += (*str - '0');
		str++;
	}
	if (*str)
		return (0);
	return (num * sign);
}

static int	update_mutex(t_philos *philo, int i)
{
	philo->philos[i].fork_mutex_other = NULL;
	philo->philos[i].mutex_died = philo->mutex_died_parent;
	philo->philos[i].fork_mutex_mine = malloc(sizeof(pthread_mutex_t));
	philo->philos[i].mutex_timer = malloc(sizeof(pthread_mutex_t));
	philo->philos[i].mutex_eat = malloc(sizeof(pthread_mutex_t));
	philo->philos[i].mutex_printer = philo->mutex_print_parent;
	if (!philo->philos[i].fork_mutex_mine || !philo->philos[i].mutex_timer
		|| !philo->philos[i].mutex_eat || !philo->philos[i].mutex_printer)
		return (0);
	if (pthread_mutex_init(philo->philos[i].mutex_eat, NULL)
		|| pthread_mutex_init(philo->philos[i].mutex_printer, NULL)
		|| pthread_mutex_init(philo->philos[i].fork_mutex_mine, NULL)
		|| pthread_mutex_init(philo->philos[i].mutex_timer, NULL))
		return (0);
	if (philo->total_philos > i + 1)
		philo->philos[i].fork_available = &philo->philos[i + 1].fork_mine;
	else
		philo->philos[i].fork_available = &philo->philos[0].fork_mine;
	philo->philos[i].count_eat = &philo->all_eat[i];
	return (1);
}

static int	get_philo_args(t_philos *philo, int *condition, int ids, char **av)
{
	philo->philos[ids - 1].id = ids;
	philo->philos[ids - 1].t_die = ft_atoi(av[0]);
	philo->philos[ids - 1].t_eat = ft_atoi(av[1]);
	philo->philos[ids - 1].t_sleep = ft_atoi(av[2]);
	*condition = -1;
	if (av[3])
		*condition = ft_atoi(av[3]);
	philo->philos[ids - 1].print = philo->print_parent;
	philo->all_eat[ids - 1] = *condition;
	philo->philos[ids - 1].fork_mine = ids;
	philo->philos[ids - 1].is_dead = &philo->dead;
	if (update_mutex(philo, ids - 1))
		return (1);
	return (0);
}

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

int	get_philos_data(t_philos *philo, char **av)
{
	int	i;

	i = 0;
	philo->total_philos = ft_atoi(av[0]);
	if (philo->total_philos <= 0)
		return (0);
	if (!param_is_correct(av))
		return (0);
	philo->philos = (t_data *)malloc(sizeof(t_data) * philo->total_philos);
	philo->print_parent = malloc (sizeof(int));//must be protected
	*philo->print_parent = 1;
	philo->dead = 0;
	philo->time_begin = ft_get_utime();
	philo->all_eat = malloc(sizeof(int) * philo->total_philos);
	if (!philo->all_eat)
		return (free(philo->philos), 0);
	philo->mutex_died_parent = malloc(sizeof(pthread_mutex_t));
	philo->mutex_print_parent = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(philo->mutex_died_parent, NULL);
	pthread_mutex_init(philo->mutex_print_parent, NULL);
	while (i < philo->total_philos)
	{
		if (!get_philo_args(philo, &philo->condition_eat, i + 1, av + 1))
			return (0);
		i++;
	}
	get_other_mutex(philo);
	return (1);
}
