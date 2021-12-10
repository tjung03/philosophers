/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recall.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjung <tjung@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 00:27:31 by tjung             #+#    #+#             */
/*   Updated: 2021/12/10 22:21:22 by tjung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	failed_free(t_philo *po, pthread_t *mnt, pthread_mutex_t *forkm)
{
	if (forkm)
		free(forkm);
	if (mnt)
		free(mnt);
	if (po)
		free(po);
}

void	recall_resources(t_common *cmn, t_philo *po, pthread_t *mnt)
{
	int	i;

	i = -1;
	while (++i < cmn->nop)
		pthread_join(po[i].tid, NULL);
	pthread_mutex_destroy(&cmn->stdout);
	i = -1;
	while (++i < cmn->nop)
		pthread_mutex_destroy(&cmn->forkm[i]);
	free(cmn->forkm);
	free(mnt);
	free(po);
}
