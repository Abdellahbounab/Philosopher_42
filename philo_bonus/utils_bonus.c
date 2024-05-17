/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abounab <abounab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 17:27:22 by abounab           #+#    #+#             */
/*   Updated: 2024/05/17 18:59:55 by abounab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
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
	num = (char *)malloc(sizeof(char) * counter + 1);
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

int	ft_atoi(char *str)
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

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joined;
	size_t	total_len;
	size_t	i;

	i = 0;
	if (s1 && s2)
	{
		total_len = ft_strlen((char *)s1) + ft_strlen((char *)s2);
		joined = (char *)malloc(sizeof(char) * (total_len + 1));
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
