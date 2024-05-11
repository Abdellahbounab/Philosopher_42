/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 12:37:21 by abounab           #+#    #+#             */
/*   Updated: 2024/05/11 20:38:37 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	read_philo_one(t_data philo)
{
	return (printf("philo(%d);%d;%d[%lld, %lld, %lld]{%p, %p}\n", philo.id, *philo.count_eat, *philo.is_dead, philo.t_die, philo.t_eat, philo.t_sleep, philo.sem_forks_checker, philo.sem_forks_checker));
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joined;
	size_t	total_len;
	size_t	i;

	i = 0;
	if (s1 && s2)
	{
		total_len = ft_strlen((char *)s1) + ft_strlen((char *)s2);
		joined = (char *) malloc(sizeof(char) * (total_len + 1));
		if (!joined)
			return (NULL);
		while (i < total_len)
		{
			if (*s1)
				*(joined + i) = *((char *)s1++);
			else
				*(joined + i) = *((char *)s2++);
			i++;
		}
		*(joined + i) = 0;
		return (joined);
	}
	return (0);
}

static char	*count_bits(int n, int *cmpt)
{
	int		counter;
	char	*num;

	counter = 0;
	if (n == 0)
		counter++;
	while (n)
	{
		counter++;
		n /= 10;
	}
	*cmpt = counter + 1;
	num = (char *) malloc (sizeof(char) * counter + 1);
	if (!num)
		return (NULL);
	num[counter] = 0;
	return (num);
}

char	*ft_itoa(int n)
{
	char	*strnum;
	int		counter;

	counter = 0;
	strnum = count_bits(n, &counter);
	if (!strnum)
		return (NULL);
	if (n == 0)
		strnum[0] = '0';
	else
	{
		counter--;
		while (--counter >= 0)
		{
			strnum[counter] = (n % 10) + '0';
			n /= 10;
		}
	}
	return (strnum);
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
	sem_wait(philo->sem_died);
	if ((*philo->is_dead))
		i = philo->id;
		// printf("entred%d\n", philo->id);
	sem_post(philo->sem_died);
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
	sem_wait(philo->sem_printer);
	if (philo->print)
		i = 1;
	printf("%d\n", i);
	sem_post(philo->sem_printer);
	return (i);
}

int	ft_printer(t_data *philo, char *str)
{
	if (check_printer(philo))
	{
		(void)str;
		// printf("%lld %d %s\n", ((ft_get_utime() - *philo->program_timer)), philo->id, str);
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

int	get_other_sem(t_philos *philo)
{
	int	i;

	i = 0;
	while (i < philo->total_philos - 1)
	{
		philo->philos[i].sem_forks_checker = philo->philos[i + 1].sem_forks_checker;
		i++;
	}
	philo->philos[i].sem_forks_checker = philo->philos[0].sem_forks_checker;
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
	
	philo->philos[ids - 1].forks_mine = 0;
	philo->philos[ids - 1].print = 1;
	philo->philos[ids - 1].n_philos = philo->total_philos;
	
	philo->philos[ids - 1].is_dead = &philo->dead;
	philo->philos[ids - 1].forks = &philo->forks_philos;

	philo->philos[ids - 1].sem_forks_checker = philo->sem_forks;
	philo->philos[ids - 1].sem_died = philo->sem_died_parent;
	
	// check for erros(success if == 0  -1 == failure)
	philo->philos[ids - 1].sem_timer = malloc (sizeof(sem_t));
	philo->philos[ids - 1].sem_eat = malloc (sizeof(sem_t));
	philo->philos[ids - 1].sem_printer = malloc (sizeof(sem_t));
	
	// check for errors
	philo->philos[ids - 1].str_printer = ft_strjoin("/sem_printer", ft_itoa(ids));
	philo->philos[ids - 1].str_timer = ft_strjoin("/sem_timer", ft_itoa(ids));
	philo->philos[ids - 1].str_eat = ft_strjoin("/sem_eat", ft_itoa(ids));
	
	philo->philos[ids - 1].sem_timer = sem_open(philo->philos[ids - 1].str_timer, O_CREAT, 0770);
	philo->philos[ids - 1].sem_eat = sem_open(philo->philos[ids - 1].str_eat, O_CREAT, 0770);
	philo->philos[ids - 1].sem_printer = sem_open(philo->philos[ids - 1].str_printer, O_CREAT, 0770);

	sem_post(philo->philos[ids - 1].sem_timer);
	sem_post(philo->philos[ids - 1].sem_eat);
	sem_post(philo->philos[ids - 1].sem_printer);
		
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
	philo->forks_philos = philo->total_philos;
	philo->arr_pid = malloc (sizeof(int) * philo->total_philos);
	philo->all_eat = malloc (sizeof(int) * philo->total_philos);
	if (!philo->all_eat)
		return (free(philo->philos), 0);
	philo->sem_died_parent = malloc (sizeof(sem_t));
	philo->sem_forks = malloc (sizeof(sem_t));
	philo->sem_died_parent = sem_open("/sem_died", O_CREAT, 0770);
	philo->sem_forks = sem_open("/sem_forks_all", O_CREAT, 0770);
	// printf("sem died : %p\n", philo->sem_died_parent);
	sem_post(philo->sem_died_parent);
	sem_post(philo->sem_forks);
	while (i < philo->total_philos)
	{
		get_philo_args(philo, &philo->condition_eat, i + 1, av + 1);
		i++;
	}
	// get_other_sem(philo);
	return (1);
}

int	compare_time(t_data *philo)
{
	long long	now;

	now = ft_get_utime();
	if (!philo_is_died(philo))
	{
		sem_wait(philo->sem_timer);
		if (now - philo->timer < philo->t_die)
			return (sem_post(philo->sem_timer), 1);
		return (sem_post(philo->sem_timer), 0);
	}
	return (-1);
}

int	is_dying(t_data *philo)
{
	if (!philo_is_died(philo))
	{
		sem_wait(philo->sem_died);
		if (*philo->count_eat)
			*philo->is_dead = philo->id;
		// printf("dead + %d\n", *philo->is_dead);
		sem_post(philo->sem_died);
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
	if (!cond)
		ft_usleep(philo, philo->timer + (philo->t_eat * 1000) / 2 - ft_get_utime());
	return (1);
}

int	handle_fork_mine(t_data *philo)
{
	if (!philo_is_died(philo))
	{
		sem_wait(philo->sem_forks_checker);
		if ((*philo->forks) && philo->n_philos == 1)
			return (sem_post(philo->sem_forks_checker), ft_printer(philo, "has taken a fork"), 
				ft_usleep(philo ,(philo->t_die - (ft_get_utime() -  philo->timer)) * 1000), is_dying(philo));
		else if ((*philo->forks))
			return (ft_printer(philo, "has taken a fork"), (*philo->forks)--, 
				sem_post(philo->sem_forks_checker), 1);
		sem_post(philo->sem_forks_checker);
		return (handle_fork_mine(philo));
	}
	return (0);
}

int	is_eating(t_data *philo)
{
	if (handle_fork_mine(philo) && !philo_is_died(philo))
	{
		sem_wait(philo->sem_forks_checker);
		if (ft_printer(philo, "has taken a fork") && ft_printer(philo,  "is eating"))
		{
			(*philo->forks)--;
			sem_post(philo->sem_forks_checker);
			sem_wait(philo->sem_timer);
			philo->timer = ft_get_utime();
			sem_post(philo->sem_timer);
			ft_usleep(philo, philo->timer + (philo->t_eat * 1000) - ft_get_utime());
			sem_wait(philo->sem_forks_checker);
			*philo->forks += 2;
			sem_post(philo->sem_forks_checker);
			return (1);
		}
		return (sem_post(philo->sem_forks_checker), 0);
	}
	return (0);
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
	if (!(philo->id % 2))
		is_thinking(philo, 0);
	while (!philo_is_died(philo) && *philo->count_eat)
	{
		if (is_eating(philo))
		{
			sem_wait(philo->sem_eat);
			if (*philo->count_eat > 0)
				(*philo->count_eat)--;
			sem_post(philo->sem_eat);
			if (is_sleeping(philo))
				is_thinking(philo, 1);
		}
	}
	// check if i have to delete all sems
	sem_close(philo->sem_forks_checker);
	sem_close(philo->sem_timer);
	sem_close(philo->sem_printer);
	sem_close(philo->sem_eat);
	sem_close(philo->sem_died);
	return (1);
}

int	free_philos(t_philos *philo)
{
	int i;

	i = 0;
	while (i < philo->total_philos)
	{
		sem_unlink(philo->philos[i].str_timer);
		// free(philo->philos[i].sem_timer);
		sem_unlink(philo->philos[i].str_eat);
		// free(philo->philos[i].sem_eat);
		sem_unlink(philo->philos[i].str_printer);
		// free(philo->philos[i].sem_printer);
		i++;
	}
	// sem_unlink("/sem_printer");
	// free(philo->sem_died_parent);
	// free(philo->philos);
	return (1);
}

int	annonce_death(t_philos *philo)
{
	int	i;

	i = 0;
	while (i < philo->total_philos)
	{
		sem_wait(philo->philos[i].sem_printer);
		philo->philos[i].print = 0;
		sem_post(philo->philos[i].sem_printer);
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
		sem_wait(philo->philos[i].sem_eat);
		if (philo->philos[i].count_eat)
		{
			sem_post(philo->philos[i].sem_eat);
			if (!compare_time(&philo->philos[i]))
				return (annonce_death(philo), is_dying(&philo->philos[i]));
		}
		sem_post(philo->philos[i].sem_eat);
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
	watcher_action(&philo);
	while (i < philo.total_philos)
	{
		waitpid(philo.arr_pid[i], NULL, 0);
		i++;
	}
	// must this function be checked in mandatory and bonus why it is here
	if (!philo_is_died(&philo.philos[i - 1]))
		is_dying(&philo.philos[i - 1]);
	free_philos(&philo);
	return (1);
}

int	run_simulation(t_philos philo)
{
	int i;
	int	pid_cpy;

	i = 0;
	get_philos_time(&philo);
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
