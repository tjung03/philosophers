/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjung <tjung@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 14:28:30 by tjung             #+#    #+#             */
/*   Updated: 2021/12/08 14:30:59 by tjung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	print_died_state(t_philo *po)
{
	long long	ms_time;

	ms_time = po->cmn->dead_time - po->cmn->start_time;
	if (!po->cmn->first_death)
	{
		po->cmn->first_death = 1;
		printf("%lldms [%d] died\n", ms_time, po->cmn->dead_p_num);
	}
}

void	print_alive_state(t_philo *po, long long ntime, char *s)
{
	long long	ms_time;

	ms_time = ntime - po->cmn->start_time;
	if (po->cmn->is_surv && !po->cmn->is_full)
		printf("%lldms [%d] %s\n", ms_time, po->p_num, s);
}

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
