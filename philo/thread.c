/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjung <tjung@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 00:46:57 by tjung             #+#    #+#             */
/*   Updated: 2021/12/11 15:08:54 by tjung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* 철학자 한 명은 포크를 둘 다 얻을 수 없고, 짝수 번호는 시작을 늦춰 초기 경합을 줄인다. */
static void	*start_dining(void *info)
{
	t_philo	*po;

	po = (t_philo *)info;
	if (po->cmn->nop == 1 && po->cmn->pme)
	{
		printf("0ms [1] has taken a fork\n");
		while (po->cmn->ttd > get_time() - po->cmn->start_time)
			usleep(1000);
		printf("%dms [1] died\n", po->cmn->ttd);
		return (NULL);
	}
	if (!(po->p_num % 2))
		while (po->cmn->tte > get_time() - po->cmn->start_time)
			usleep(1000);
	while (po->cmn->is_surv && po->cmn->pme)
	{
		pick_up(po);
		eat(po);
		do_sleep(po);
		think(po);
	}
	return (NULL);
}

/* 식사 횟수 완료는 생존 시간 초과와 별도 모니터에서 종료 조건으로 확인한다. */
static void	*monitoring_must_eat(void *info)
{
	t_common	*mme;

	mme = (t_common *)info;
	while (mme->is_surv)
	{
		if (mme->full_cnt == mme->nop)
			mme->is_surv = 0;
		usleep(1000);
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
		ms_time = get_time();
		if (mnt[i].cmn->ttd < ms_time - mnt[i].hunger_time)
		{
			mnt[i].cmn->is_surv = 0;
			pthread_mutex_lock(&mnt[i].cmn->stdout);
			printf("%lldms\t[%d]\t%s\n", \
			ms_time - mnt[i].cmn->start_time, mnt[i].p_num, "died");
			pthread_mutex_unlock(&mnt[i].cmn->stdout);
		}
		if (mnt[i].cmn->nop == i + 1)
			i = -1;
		usleep(1000);
	}
	return (NULL);
}

void	create_thread(t_common *cmn, t_philo *po, pthread_t *mnt_id)
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
		pthread_create(&mnt_id[0], NULL, monitoring, (void *)po);
		pthread_detach(mnt_id[0]);
		if (cmn->pme > 0)
		{
			pthread_create(&mnt_id[1], NULL, monitoring_must_eat, (void *)cmn);
			pthread_detach(mnt_id[1]);
		}
	}
}
