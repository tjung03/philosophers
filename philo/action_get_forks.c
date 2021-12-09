/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_get_forks.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjung <tjung@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 16:47:20 by tjung             #+#    #+#             */
/*   Updated: 2021/12/09 21:47:54 by tjung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	put_down(t_philo *po)
{
	pthread_mutex_unlock(&po->cmn->forkm[po->lf]);
	pthread_mutex_unlock(&po->cmn->forkm[po->rf]);
	po->cmn->forks[po->lf] = 1;
	po->cmn->forks[po->rf] = 1;
}

static void	odd_get_forks(t_philo *po, int fst, int snd)
{
	pthread_mutex_lock(&po->cmn->forkm[fst]);
	if (!po->cmn->is_surv || po->cmn->is_full)
		return ;
	po->new_time = get_time();
	pthread_mutex_lock(&po->cmn->stdout);
	print_alive_state(po, po->new_time, "has taken a fork");
	pthread_mutex_unlock(&po->cmn->stdout);
	pthread_mutex_lock(&po->cmn->forkm[snd]);
	if (!po->cmn->is_surv || po->cmn->is_full)
		return ;
	po->new_time = get_time();
	pthread_mutex_lock(&po->cmn->stdout);
	print_alive_state(po, po->new_time, "has taken a fork");
	pthread_mutex_unlock(&po->cmn->stdout);
}

void	odd_pick_up(t_philo *po)
{
	if (!po->cmn->is_surv || po->cmn->is_full)
	{
		if (!po->cmn->is_surv)
		{
			pthread_mutex_lock(&po->cmn->stdout);
			print_died_state(po);
			pthread_mutex_unlock(&po->cmn->stdout);
		}
		return ;
	}
	if (po->p_num % 2)
		odd_get_forks(po, po->lf, po->rf);
	else
		odd_get_forks(po, po->rf, po->lf);
}

static void	even_get_forks(t_philo *po)
{
	po->cmn->forks[po->lf] = 0;
	po->cmn->forks[po->rf] = 0;
	pthread_mutex_lock(&po->cmn->forkm[po->lf]);
	pthread_mutex_lock(&po->cmn->forkm[po->rf]);
	po->new_time = get_time();
	pthread_mutex_lock(&po->cmn->stdout);
	print_alive_state(po, po->new_time, "has taken a fork");
	print_alive_state(po, po->new_time, "has taken a fork");
	pthread_mutex_unlock(&po->cmn->stdout);
}

void	even_pick_up(t_philo *po)
{
	while (po->cmn->is_surv)
	{
		if (po->cmn->is_full)
			break ;
		if (po->cmn->forks[po->lf] && po->cmn->forks[po->rf])
		{
			even_get_forks(po);
			break ;
		}
	}
	if (!po->cmn->is_surv)
	{
		pthread_mutex_lock(&po->cmn->stdout);
		print_died_state(po);
		pthread_mutex_unlock(&po->cmn->stdout);
	}
}
