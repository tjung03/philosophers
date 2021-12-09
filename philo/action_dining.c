/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_dining.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjung <tjung@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 16:29:51 by tjung             #+#    #+#             */
/*   Updated: 2021/12/09 21:45:16 by tjung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	think(t_philo *po)
{
	pthread_mutex_lock(&po->cmn->stdout);
	print_alive_state(po, po->new_time, "is thinking");
	pthread_mutex_unlock(&po->cmn->stdout);
}

static void	do_sleep(t_philo *po)
{
	long long	stime;
	
	stime = po->new_time;
	pthread_mutex_lock(&po->cmn->stdout);
	print_alive_state(po, po->new_time, "is sleeping");
	pthread_mutex_unlock(&po->cmn->stdout);
	po->new_time = waiting(po, stime, po->cmn->tts);
	if (!po->cmn->is_surv)
	{
		pthread_mutex_lock(&po->cmn->stdout);
		print_died_state(po);
		pthread_mutex_unlock(&po->cmn->stdout);
	}
}

static void	check_full(t_philo *po)
{
	po->eat_cnt++;
	if (po->cmn->pme != -1 && po->eat_cnt == po->cmn->pme)
		po->cmn->full_cnt += ++po->full;
	if (po->cmn->full_cnt == po->cmn->nop)
		po->cmn->is_full = 1;
}

static void	eat(t_philo *po)
{
	po->hunger_time = po->new_time;
	pthread_mutex_lock(&po->cmn->stdout);
	print_alive_state(po, po->new_time, "is eating");
	pthread_mutex_unlock(&po->cmn->stdout);
	if (po->cmn->is_surv)
		check_full(po);
	po->new_time = waiting(po, po->hunger_time, po->cmn->tte);
	put_down(po);
	if (!po->cmn->is_surv)
	{
		pthread_mutex_lock(&po->cmn->stdout);
		print_died_state(po);
		pthread_mutex_unlock(&po->cmn->stdout);
	}
}

int	action_dining(t_philo *po)
{
	if (po->cmn->nop % 2)
		odd_pick_up(po);
	else
		even_pick_up(po);
	eat(po);
	do_sleep(po);
	think(po);
	if (po->cmn->is_full)
		return (0);
	return (po->cmn->is_surv);
}
