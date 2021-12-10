/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjung <tjung@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 14:28:27 by tjung             #+#    #+#             */
/*   Updated: 2021/12/10 22:34:10 by tjung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	simulation(t_common *cmn)
{
	t_philo		*po;
	pthread_t	*mnt_id;

	po = (t_philo *)malloc(sizeof(t_philo) * cmn->nop);
	mnt_id = (pthread_t *)malloc(sizeof(pthread_t) * 2);
	cmn->forkm = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * cmn->nop);
	if (!po || !mnt_id || !cmn->forkm)
	{
		failed_free(po, mnt_id, cmn->forkm);
		return (print_error(1, "Malloc Error!"));
	}
	init(cmn, po);
	create_thread(cmn, po, mnt_id);
	recall_resources(cmn, po, mnt_id);
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
