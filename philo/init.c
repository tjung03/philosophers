/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjung <tjung@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 00:34:05 by tjung             #+#    #+#             */
/*   Updated: 2021/12/10 22:22:11 by tjung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init(t_common *cmn, t_philo *po)
{
	int	idx;

	pthread_mutex_init(&cmn->stdout, NULL);
	cmn->is_surv = 1;
	memset(po, 0, sizeof(*po));
	idx = -1;
	while (++idx < cmn->nop)
	{
		pthread_mutex_init(&cmn->forkm[idx], NULL);
		po[idx].cmn = cmn;
		po[idx].p_num = idx + 1;
		po[idx].rf = idx;
		po[idx].lf = idx - 1;
		if (!idx)
			po[idx].lf = cmn->nop - 1;
	}
}
