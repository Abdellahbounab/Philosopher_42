/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 19:50:33 by abounab           #+#    #+#             */
/*   Updated: 2024/05/06 22:40:48 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	read_philo_one(t_data philo)
{
	return (printf("philo(%d);%d;%d[%lld, %lld, %lld]{%p, %p}\n", philo.id, philo.count_eat, *philo.is_dead, philo.t_die, philo.t_eat, philo.t_sleep, philo.fork_mutex_mine, philo.fork_mutex_other));
}

int read_many_philo(t_philos philo)
{
	int i;

	i = 0;
	while (i < philo.total_philos)
		read_philo_one(philo.philos[i++]);
	return (1);
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
	return (num * sign);
}

int	get_other_mutex(t_philos *philo, int total)
{
	int	i;

	i = 0;
	while (i < total - 1)
	{
		philo->philos[i].fork_mutex_other = philo->philos[i + 1].fork_mutex_mine;
		i++;
	}
	philo->philos[i].fork_mutex_other = philo->philos[0].fork_mutex_mine;
	return (1);
}

int	philo_is_died(t_data *philo)
{
	int i;

	i = 0;
	if (*philo->is_dead)
		return (0);
	pthread_mutex_lock(philo->mutex_died);
	if (!(*philo->is_dead))
		i = 1;
	pthread_mutex_unlock(philo->mutex_died);
	return (i);
}

long long ft_get_utime(void)
{
	// 1 s 	  = 1000000 micros
	// 1 mls  = 1000 micros
	// 1 s	  = 1000 mls
	struct timeval now;

	gettimeofday(&now, NULL);
	return (((now.tv_sec * 1000000) + now.tv_usec));
}

int get_philo_args(t_philos *philo, int *condition, int ids, char **av)
{
	philo->philos[ids - 1].id = ids;
	philo->philos[ids - 1].t_die = ft_atoi(av[0]);
	philo->philos[ids - 1].t_eat = ft_atoi(av[1]);
	philo->philos[ids - 1].t_sleep = ft_atoi(av[2]);
	philo->philos[ids - 1].fork_mine = 1;
	philo->philos[ids - 1].is_dead = &philo->dead;
	philo->philos[ids - 1].fork_mutex_other = NULL;
	philo->philos[ids - 1].fork_mutex_mine = malloc(sizeof(pthread_mutex_t));
	philo->philos[ids - 1].mutex_died = malloc(sizeof(pthread_mutex_t));
	if (!philo->philos[ids - 1].fork_mutex_mine)
		printf("error malloc");
	if (pthread_mutex_init(philo->philos[ids - 1].fork_mutex_mine, NULL))
		printf("mutex failed(%d)\n", philo->philos[ids - 1].id);
	
	// if conditiion
	if (philo->total_philos > ids)
		philo->philos[ids - 1].fork_available = &philo->philos[ids].fork_mine;
	else
		philo->philos[ids - 1].fork_available = &philo->philos[0].fork_mine;
	*condition = -1;
	if (av[3])
		*condition = ft_atoi(av[3]);
	philo->philos[ids - 1].count_eat = *condition;
	return (1);
}

// args (num_philos, t_die, t_eat, t_sleep, [optional]num_condition_of_eat)

int	get_philos_data(t_philos *philo, char **av)
{
	int	i;

	i = 0;
	philo->total_philos = ft_atoi(av[0]);
	// malloced
	if (philo->total_philos < 0)
		return (0);
	philo->philos = (t_data *) malloc (sizeof(t_data) * philo->total_philos);
	philo->dead = 0;
	while (i < philo->total_philos)
	{
		get_philo_args(philo, &philo->condition_eat, i + 1, av + 1);
		i++;
	}
	get_other_mutex(philo, philo->total_philos);
	return (1);
}

int	compare_time(long long philo_timer, long long other_time)
{
	long long	now;

	now = ft_get_utime();
	if (now - philo_timer <= other_time * 1000)
		return (1);
	return (0);
}

int	is_dying(t_data *philo)
{
	if (philo_is_died(philo))
	{
		pthread_mutex_lock(philo->mutex_died);
		*philo->is_dead = philo->id;
		printf("%lld %d is died\n",(ft_get_utime() / 1000) % 10000, philo->id);
		pthread_mutex_unlock(philo->mutex_died);
	}
	return (0);
}

int	is_sleeping(t_data *philo)
{
	if (philo_is_died(philo) && !compare_time(philo->t_sleep, philo->t_die))
	{
		usleep(philo->t_sleep * 1000);
		printf("%lld %d is sleeping\n", (ft_get_utime() / 1000) % 10000, philo->id);
		return (1);
	}
	else
		return (is_dying(philo));
}

int	is_thinking(t_data *philo)
{
	long long	now;

	now = ft_get_utime();
	if (philo_is_died(philo) && !compare_time(philo->timer, philo->t_die))
	{
		printf("%lld %d is thinking\n", (ft_get_utime() / 1000) % 10000, philo->id);
		usleep(((now - philo->timer) % 100));
	}
	return (1);
}

int	is_eating(t_data *philo)
{
	// adding time stamp to get the time before they die
	if (philo_is_died(philo) && compare_time(philo->timer, philo->t_die))
	{
		pthread_mutex_lock(philo->fork_mutex_mine);
		if (philo_is_died(philo) && compare_time(philo->timer, philo->t_die))
		{
			printf("%lld %d has fork right\n",(ft_get_utime() / 1000) % 10000, philo->id);
			pthread_mutex_lock(philo->fork_mutex_other);
			if (philo_is_died(philo))
			{
				printf("%lld %d has fork left\n", (ft_get_utime() / 1000) % 10000, philo->id);
				printf("%lld %d is eating\n", (ft_get_utime() / 1000) % 10000,philo->id);
				usleep(philo->t_eat * 1000);
				pthread_mutex_unlock(philo->fork_mutex_other);
				pthread_mutex_unlock(philo->fork_mutex_mine);
				// printf("finished %d\n", philo->id);
				philo->timer = ft_get_utime();
				return (1);
			}
			return (printf("here1[%d]\n", philo->id), is_dying(philo));
		}
		return (printf("here2[%d]\n", philo->id), is_dying(philo));
	}
	return (printf("here3[%d]\n", philo->id), is_dying(philo));
	// condition else where he died without eating
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
		i++;
	}
	return (1);
}

int	turn_action(t_data *philo)
{
	// iwould get the forks (check if fork[i] = 1 && fork[i + 1] = 1 to start eat)
	// if (i - 1 == eat) (i == sleep) (i + 1 == thinking)
	// if (philo->fork_available && )
	// while (1)
	// {
		
	// }
	// printf("id (%d)[%p, %p]\n", philo->id, &philo->fork_mutex_mine, philo->fork_mutex_other);
	if (!(philo->id % 2))
		usleep(100);
	while (philo_is_died(philo) && philo->count_eat)
	{
		if (is_eating(philo))
		{
			if (philo->count_eat > 0)
				philo->count_eat--;
			if (!is_sleeping(philo))
				// is_thinking(philo);
				break;
		}
	}
	// printf("%d out\n", philo->id);
		// *philo->fork_available = 1;
	// if (philo->philos[i])
	return (1);
}

int	waiting_threads(t_philos philo)
{
	int i;

	i = 0;
	get_philos_time(&philo);
	while (i < philo.total_philos)
	{
		pthread_join(philo.philos[i].thread, NULL);
		printf("thread[%d] is ended\n", i + 1);
		i++;
	}
	return (1);
}


int	create_threads(t_philos philo)
{
	int i;

	i = 0;
	get_philos_time(&philo);
	while (i < philo.total_philos)
	{
		pthread_create(&philo.philos[i].thread, NULL, (void *)turn_action, (void *)&philo.philos[i]);
		i++;
	}
	waiting_threads(philo);
	return (1);
}

int	run_simulation(t_philos philo)
{
	create_threads(philo);
	// wait for threads and destroy mutexes
	return (1);
}

int	main(int ac, char **av)
{
	t_philos	philo;

	if (ac >= 5 && ac <= 6)
	{
		get_philos_data(&philo, av + 1);
		// read_many_philo(philo);
		run_simulation(philo);
	}
}
