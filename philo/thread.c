/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjung <tjung@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 00:46:57 by tjung             #+#    #+#             */
/*   Updated: 2021/12/09 21:33:16 by tjung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*start_dining(void *info)
{
	t_philo	*po;

	po = (t_philo *)info;
	if (po->cmn->nop == 1 && po->cmn->pme)
	{
		printf("0ms [1] has taken a fork\n");
		printf("%dms [1] died\n", po->cmn->ttd);
		return (NULL);
	}
	if (!(po->p_num % 2))
		waiting(po, po->cmn->start_time, po->cmn->tte);
	while (po->cmn->is_surv)
	{
		if (!po->cmn->pme)
			break ;
		if (!action_dining(po))
			break ;
	}
	return (NULL);
}

static void	*monitoring(void *info)
{
	t_philo		*mnt;
	long long	ms_time;
	int			i;

	mnt = (t_philo *)info;
	i = -1;
	while (mnt[++i].cmn->is_surv)
	{
		if (mnt[i].cmn->is_full)
			break ;
		ms_time = get_time();
		if (ms_time - mnt[i].hunger_time >= mnt[i].cmn->ttd)
		{
			if (mnt[i].cmn->is_surv)
			{
				mnt[i].cmn->is_surv = 0;
				mnt[i].cmn->dead_p_num = mnt[i].p_num;
				mnt[i].cmn->dead_time = ms_time;
			}
			break ;
		}
		if (mnt[i].cmn->nop == i + 1)
			i = -1;
	}
	return (NULL);
}

void	create_thread(t_common *cmn, t_philo *po, pthread_t *mnt_tid)
{
	int	i;

	i = -1;
	cmn->start_time = get_time();
	while (++i < cmn->nop)
	{
		po[i].hunger_time = cmn->start_time;
		pthread_create(&po[i].tid, NULL, start_dining, (void *)&po[i]);
	}
	if (cmn->nop > 1 && (cmn->pme > 0 || cmn->pme == -1))
	{
		pthread_create(mnt_tid, NULL, monitoring, (void *)po);
		pthread_detach(*mnt_tid);
	}
}
