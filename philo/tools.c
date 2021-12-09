/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjung <tjung@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 14:28:30 by tjung             #+#    #+#             */
/*   Updated: 2021/12/09 21:30:25 by tjung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time(void)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((long long)(now.tv_sec * 1000 + now.tv_usec / 1000));
}

long long	waiting(t_philo *po, long long start, long long standard)
{
	long long	curr;
	
	while (po->cmn->is_surv)
	{
		if (po->cmn->is_full)
			break ;
		curr = get_time();
		if (curr - start >= standard)
			break ;
	}
	if (!po->cmn->is_surv || po->cmn->is_full)
		return (0);
	return (curr);
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
