/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recall.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjung <tjung@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 00:27:31 by tjung             #+#    #+#             */
/*   Updated: 2021/12/09 16:48:57 by tjung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_malloc_by_failed(t_common *cmn, t_philo *po)
{
	if (cmn->forkm)
		free(cmn->forkm);
	if (cmn->forks)
		free(cmn->forks);
	if (po)
		free(po);
}

void	recall_resources(t_common *cmn, t_philo *po)
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
	free(cmn->forks);
	free(po);
}
