/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjung <tjung@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 14:28:30 by tjung             #+#    #+#             */
/*   Updated: 2021/12/11 00:49:58 by tjung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print_error(int ret, char *s)
{
	if (ret == 1)
		printf("%s\n", s);
	return (ret);
}

long long	get_time(void)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((long long)(now.tv_sec * 1000 + now.tv_usec / 1000));
}

int	my_atoi(char *s)
{
	int	num;
	int	i;

	num = 0;
	i = 0;
	while (s[i])
	{
		num = (num * 10) + (s[i] - 48);
		i++;
	}
	return (num);
}
