/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjung <tjung@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 17:00:01 by tjung             #+#    #+#             */
/*   Updated: 2021/12/09 17:01:28 by tjung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print_error(int ret, char *s)
{
	if (ret == 1)
		printf("%s\n", s);
	return (ret);
}

void	print_alive_state(t_philo *po, long long ntime, char *s)
{
	long long	ms_time;

	ms_time = ntime - po->cmn->start_time;
	if (po->cmn->is_surv && !po->cmn->is_full)
		printf("%lldms [%d] %s\n", ms_time, po->p_num, s);
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
