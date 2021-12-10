/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_dining.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjung <tjung@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 16:29:51 by tjung             #+#    #+#             */
/*   Updated: 2021/12/11 01:23:56 by tjung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	think(t_philo *po)
{
	pthread_mutex_lock(&po->cmn->stdout);
	if (po->cmn->is_surv)
		printf("%lldms\t[%d]\t%s\n", \
		get_time() - po->cmn->start_time, po->p_num, "is thinking");
	pthread_mutex_unlock(&po->cmn->stdout);
}

void	do_sleep(t_philo *po)
{
	long long	stime;

	pthread_mutex_lock(&po->cmn->stdout);
	stime = get_time();
	if (po->cmn->is_surv)
		printf("%lldms\t[%d]\t%s\n", \
		stime - po->cmn->start_time, po->p_num, "is sleeping");
	pthread_mutex_unlock(&po->cmn->stdout);
	while (po->cmn->tts > get_time() - stime)
		usleep(1000);
}

void	eat(t_philo *po)
{
	pthread_mutex_lock(&po->cmn->stdout);
	po->hunger_time = get_time();
	if (po->cmn->is_surv)
		printf("%lldms\t[%d]\t%s\n", \
		po->hunger_time - po->cmn->start_time, po->p_num, "is eating");
	po->eat_cnt++;
	if (po->cmn->pme != -1 && po->eat_cnt == po->cmn->pme)
		po->cmn->full_cnt += ++po->full;
	pthread_mutex_unlock(&po->cmn->stdout);
	while (po->cmn->tte > get_time() - po->hunger_time)
		usleep(1000);
	pthread_mutex_unlock(&po->cmn->forkm[po->lf]);
	pthread_mutex_unlock(&po->cmn->forkm[po->rf]);
}

void	pick_up(t_philo *po)
{
	pthread_mutex_lock(&po->cmn->forkm[po->rf]);
	pthread_mutex_lock(&po->cmn->stdout);
	if (po->cmn->is_surv)
		printf("%lldms\t[%d]\t%s\n", \
		get_time() - po->cmn->start_time, po->p_num, "has taken a fork");
	pthread_mutex_unlock(&po->cmn->stdout);
	pthread_mutex_lock(&po->cmn->forkm[po->lf]);
	pthread_mutex_lock(&po->cmn->stdout);
	if (po->cmn->is_surv)
		printf("%lldms\t[%d]\t%s\n", \
		get_time() - po->cmn->start_time, po->p_num, "has taken a fork");
	pthread_mutex_unlock(&po->cmn->stdout);
}
