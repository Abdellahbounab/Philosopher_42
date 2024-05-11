/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 19:50:33 by abounab           #+#    #+#             */
/*   Updated: 2024/05/11 17:30:47 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	read_philo_one(t_data philo)
{
	return (printf("philo(%d);%d;%d[%lld, %lld, %lld]{%p, %p}\n", philo.id, *philo.count_eat, *philo.is_dead, philo.t_die, philo.t_eat, philo.t_sleep, philo.fork_mutex_mine, philo.fork_mutex_other));
}

int read_many_philo(t_philos philo)
{
	int i;

	i = 0;
	while (i < philo.total_philos)
		read_philo_one(philo.philos[i++]);
	return (1);
}

int	philo_is_died(t_data *philo)
{
	int i;

	i = 0;
	pthread_mutex_lock(philo->mutex_died);
	if ((*philo->is_dead))
		i = philo->id;
	pthread_mutex_unlock(philo->mutex_died);
	return (i);
}

long long ft_get_utime(void)
{
	struct timeval now;

	gettimeofday(&now, NULL);
	return (((now.tv_sec * 1000000) + now.tv_usec) / 1000);
}

int	check_printer(t_data *philo)
{
	int i;

	i = 0;
	pthread_mutex_lock(philo->mutex_printer);
	if (philo->print)
		i = 1;
	pthread_mutex_unlock(philo->mutex_printer);
	return (i);
}

int	ft_printer(t_data *philo, char *str)
{
	if (check_printer(philo))
	{
		printf("%lld %d %s\n", ((ft_get_utime() - *philo->program_timer)), philo->id, str);
		return (1);
	}
	return (0);
}

int	ft_atoi(char *str)
{
	int	num;
	int sign;

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

int	get_other_mutex(t_philos *philo)
{
	int	i;

	i = 0;
	while (i < philo->total_philos - 1)
	{
		philo->philos[i].fork_mutex_other = philo->philos[i + 1].fork_mutex_mine;
		i++;
	}
	philo->philos[i].fork_mutex_other = philo->philos[0].fork_mutex_mine;
	return (1);
}

void	ft_usleep(t_data *philo ,long long timer)
{
	long long	val;

	val = ft_get_utime() * 1000;
	while (ft_get_utime() * 1000 < val + timer && check_printer(philo))
		usleep(70);
}

int get_philo_args(t_philos *philo, int *condition, int ids, char **av)
{
	philo->philos[ids - 1].id = ids;
	philo->philos[ids - 1].t_die = ft_atoi(av[0]);
	philo->philos[ids - 1].t_eat = ft_atoi(av[1]);
	philo->philos[ids - 1].t_sleep = ft_atoi(av[2]);
	
	philo->philos[ids - 1].fork_mine = ids;
	philo->philos[ids - 1].print = 1;
	philo->philos[ids - 1].is_dead = &philo->dead;
	philo->philos[ids - 1].fork_mutex_other = NULL;
	
	philo->philos[ids - 1].fork_mutex_mine = malloc(sizeof(pthread_mutex_t));
	philo->philos[ids - 1].mutex_timer = malloc(sizeof(pthread_mutex_t));
	philo->philos[ids - 1].mutex_eat = malloc(sizeof(pthread_mutex_t));
	philo->philos[ids - 1].mutex_printer = malloc(sizeof(pthread_mutex_t));
	
	philo->philos[ids - 1].mutex_died = philo->mutex_died_parent;
	if (!philo->philos[ids - 1].fork_mutex_mine)
		printf("error malloc");
	if (pthread_mutex_init(philo->philos[ids - 1].mutex_eat, NULL))
		printf("mutex failed(%d)\n", philo->philos[ids - 1].id);
	if (pthread_mutex_init(philo->philos[ids - 1].mutex_printer, NULL))
		printf("mutex failed(%d)\n", philo->philos[ids - 1].id);
	if (pthread_mutex_init(philo->philos[ids - 1].fork_mutex_mine, NULL))
		printf("mutex failed(%d)\n", philo->philos[ids - 1].id);
	if (pthread_mutex_init(philo->philos[ids - 1].mutex_timer, NULL))
		printf("mutex failed(%d)\n", philo->philos[ids - 1].id);
		
	if (philo->total_philos > ids)
		philo->philos[ids - 1].fork_available = &philo->philos[ids].fork_mine;
	else
		philo->philos[ids - 1].fork_available = &philo->philos[0].fork_mine;
	*condition = -1;
	if (av[3])
		*condition = ft_atoi(av[3]);
	philo->all_eat[ids - 1] = *condition;
	philo->philos[ids - 1].count_eat = &philo->all_eat[ids - 1];
	return (1);
}

int	param_is_correct(char **av)
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
	philo->philos = (t_data *) malloc (sizeof(t_data) * philo->total_philos);
	philo->dead = 0;
	philo->time_begin = ft_get_utime();
	philo->all_eat = malloc (sizeof(int) * philo->total_philos);
	if (!philo->all_eat)
		return (free(philo->philos), 0);
	philo->mutex_died_parent = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(philo->mutex_died_parent, NULL);
	while (i < philo->total_philos)
	{
		get_philo_args(philo, &philo->condition_eat, i + 1, av + 1);
		i++;
	}
	get_other_mutex(philo);
	return (1);
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

int	is_dying(t_data *philo)
{
	if (!philo_is_died(philo))
	{
		pthread_mutex_lock(philo->mutex_died);
		if (*philo->count_eat)
			*philo->is_dead = philo->id;
		pthread_mutex_unlock(philo->mutex_died);
		if (*philo->count_eat)
			printf("%s%lld %d died%s\n", RED, ((ft_get_utime() - *philo->program_timer)), philo->id, DEFAULT);
	}
	return (0);
}

int	is_sleeping(t_data *philo)
{
	if (!ft_printer(philo, "is sleeping"))
		return (0);
	if (philo->t_sleep + (ft_get_utime() - philo->timer) < philo->t_die)
	{
		ft_usleep(philo, philo->timer + philo->t_sleep * 1000 - ft_get_utime());
		if (!philo_is_died(philo))
			return (1);
		return (0);
	}
	return (0);
}

int	is_thinking(t_data *philo, int cond)
{
	ft_printer(philo, "is thinking");
	if (!cond && !(philo->id % 2))
		ft_usleep(philo, philo->timer + (philo->t_eat * 1000) / 2 - ft_get_utime());
	return (1);
}

int	handle_fork_mine(t_data *philo)
{
	if (!philo_is_died(philo) && *philo->fork_available == philo->id)
	{
		ft_usleep(philo ,(philo->t_die - (ft_get_utime() -  philo->timer)) * 1000);
		return (is_dying(philo));
	}
	return (1);
}

int	is_eating(t_data *philo)
{
	pthread_mutex_lock(philo->fork_mutex_mine);
	ft_printer(philo, "has taken a fork");
	if (handle_fork_mine(philo) && !philo_is_died(philo))
	{
		pthread_mutex_lock(philo->fork_mutex_other);
		if (ft_printer(philo, "has taken a fork") && ft_printer(philo,  "is eating"))
		{
			pthread_mutex_lock(philo->mutex_timer);
			philo->timer = ft_get_utime();
			pthread_mutex_unlock(philo->mutex_timer);
			ft_usleep(philo, philo->timer + (philo->t_eat * 1000) - ft_get_utime());
			return (pthread_mutex_unlock(philo->fork_mutex_other),
				pthread_mutex_unlock(philo->fork_mutex_mine), 1);
		}
		return (pthread_mutex_unlock(philo->fork_mutex_other),
			pthread_mutex_unlock(philo->fork_mutex_mine), 0);
	}
	return (pthread_mutex_unlock(philo->fork_mutex_mine), 0);
}


int	get_philos_time(t_philos *philo)
{
	int	i;
	long long now;

	i = 0;
	now = ft_get_utime();
	while (i < philo->total_philos)
	{
		philo->philos[i].timer = now;
		philo->philos[i].program_timer = &philo->time_begin;
		i++;
	}
	return (1);
}

int	turn_action(t_data *philo)
{
	is_thinking(philo, 0);
	while (!philo_is_died(philo) && *philo->count_eat)
	{
		if (is_eating(philo))
		{
			pthread_mutex_lock(philo->mutex_eat);
			if (*philo->count_eat > 0)
				(*philo->count_eat)--;
			pthread_mutex_unlock(philo->mutex_eat);
			if (is_sleeping(philo))
				is_thinking(philo, 1);
		}
	}
	return (1);
}

int	free_philos(t_philos *philo)
{
	int i;

	i = 0;
	while (i < philo->total_philos)
	{
		pthread_mutex_destroy(philo->philos[i].fork_mutex_mine);
		free(philo->philos[i].fork_mutex_mine);
		pthread_mutex_destroy(philo->philos[i].mutex_timer);
		free(philo->philos[i].mutex_timer);
		pthread_mutex_destroy(philo->philos[i].mutex_eat);
		free(philo->philos[i].mutex_eat);
		pthread_mutex_destroy(philo->philos[i].mutex_printer);
		free(philo->philos[i].mutex_printer);
		i++;
	}
	pthread_mutex_destroy(philo->mutex_died_parent);
	free(philo->mutex_died_parent);
	free(philo->philos);
	return (1);
}

int	annonce_death(t_philos *philo)
{
	int	i;

	i = 0;
	while (i < philo->total_philos)
	{
		pthread_mutex_lock(philo->philos[i].mutex_printer);
		philo->philos[i].print = 0;
		pthread_mutex_unlock(philo->philos[i].mutex_printer);
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
		if (!pthread_mutex_lock(philo->philos[i].mutex_eat) && philo->philos[i].count_eat)
		{
			pthread_mutex_unlock(philo->philos[i].mutex_eat);
			if (!compare_time(&philo->philos[i]))
				return (annonce_death(philo), is_dying(&philo->philos[i]));
		}
		pthread_mutex_unlock(philo->philos[i].mutex_eat);
		if (i < philo->total_philos - 1)
			i++;
		else
			i = 0;
	}
	return (1);
}

int	waiting_threads(t_philos philo)
{
	int i;

	i = 0;
	while (i < philo.total_philos)
	{
		pthread_join(philo.philos[i].thread, NULL);
		i++;
	}
	// i forgot why i craete this condiiton , must be checked later
	if (!philo_is_died(&philo.philos[i - 1]))
		is_dying(&philo.philos[i - 1]);
	pthread_join(philo.watcher, NULL);
	free_philos(&philo);
	return (1);
}

int	run_simulation(t_philos philo)
{
	int i;

	i = 0;
	get_philos_time(&philo);
	while (i < philo.total_philos)
	{
		pthread_create(&philo.philos[i].thread, NULL, (void *)turn_action, (void *)&philo.philos[i]);
		i++;
	}
	pthread_create(&philo.watcher, NULL, (void *)watcher_action, (void *)&philo);
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
