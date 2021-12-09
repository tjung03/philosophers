/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjung <tjung@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 14:28:27 by tjung             #+#    #+#             */
/*   Updated: 2021/12/09 04:20:51 by tjung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	simulation(t_common *cmn)
{
	t_philo		*po;
	pthread_t	mnt_tid;

	po = (t_philo *)malloc(sizeof(t_philo) * cmn->nop);
	cmn->forkm = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * cmn->nop);
	cmn->forks = (int *)malloc(sizeof(int) * cmn->nop);
	if (!po || !cmn->forkm || !cmn->forks)
	{
		free_malloc_by_failed(cmn, po);
		return (print_error(1, "Malloc Error!"));
	}
	init(cmn, po);
	create_thread(cmn, po, &mnt_tid);
	recall_resources(cmn, po);
	po = NULL;
	return (0);
}

int	main(int ac, char **av)
{
	t_common	cmn;

	memset(&cmn, 0, sizeof(cmn));
	if (ac != 5 && ac != 6)
		return (print_error(1, "Parsing Error!"));
	if (get_options(&cmn, ac, av))
		return (print_error(1, "Get option Error!"));
	if (simulation(&cmn))
		return (print_error(1, "Simulation Error!"));
	return (0);
}
