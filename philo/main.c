/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjung <tjung@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 14:28:27 by tjung             #+#    #+#             */
/*   Updated: 2021/12/08 21:31:59 by tjung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_malloc(t_philo *po, t_monitor *mo, pthread_mutex_t *fm, int *fs)
{
	if (po)
		free(po);
	if (mo)
		free(mo);
	if (fm)
		free(fm);
	if (fs)
		free(fs);
}

void	init_values(t_common *cmn, t_philo *po, t_monitor *mnt, int *i)
{
	pthread_mutex_init(&cmn->forkm[*i], NULL);
	cmn->forks[*i] = 1;
	po[*i].cmn = cmn;
	po[*i].p_num = *i + 1;
	po[*i].rf = *i;
	if (*i != 0)
		po[*i].lf = *i - 1;
	else
		po[*i].lf = cmn->nop - 1;
	mnt[*i].check_died = &cmn->check_died;
	mnt[*i].hunger_time = &po[*i].hunger_time;
	mnt[*i].dead_time = &cmn->dead_time;
	mnt[*i].dead_p_num = &cmn->dead_p_num;
	mnt[*i].is_surv = &cmn->is_surv;
	mnt[*i].is_full = &cmn->is_full;
	mnt[*i].ttd = cmn->ttd;
	mnt[*i].m_num = *i + 1;
}

void	init(t_common *cmn, t_philo *po, t_monitor *mnt)
{
	int	i;

	pthread_mutex_init(&cmn->stdout, NULL);
	pthread_mutex_init(&cmn->check_died, NULL);
	memset(po, 0, sizeof(*po));
	i = -1;
	while (++i < cmn->nop)
		init_values(cmn, po, mnt, &i);
}

void	do_sleep(t_philo *po)
{
	long long	stime;

	stime = po->new_time;
	while (1)
	{
		if (!po->cmn->is_surv || po->cmn->is_full)
		{
			if (!po->cmn->is_surv)
			{
				pthread_mutex_lock(&po->cmn->stdout);
				print_died_state(po);
				pthread_mutex_unlock(&po->cmn->stdout);
			}
			break ;
		}
		po->new_time = get_time();
		if (po->new_time - stime >= po->cmn->tts)
		{
			pthread_mutex_lock(&po->cmn->stdout);
			print_alive_state(po, po->new_time, "is thinking");
			pthread_mutex_unlock(&po->cmn->stdout);
			break ;
		}
	}
}

void	put_down(t_philo *po)
{
	pthread_mutex_unlock(&po->cmn->forkm[po->rf]);
	pthread_mutex_unlock(&po->cmn->forkm[po->lf]);
	po->cmn->forks[po->rf] = 1;
	po->cmn->forks[po->lf] = 1;
}

void	check_full(t_philo *po)
{
	po->eat_cnt++;
	if (po->cmn->pme != -1 && po->eat_cnt == po->cmn->pme)
		po->cmn->full_cnt += ++po->full;
	if (po->cmn->full_cnt == po->cmn->nop)
		po->cmn->is_full = 1;
}

void	eat(t_philo *po)
{
	po->hunger_time = po->new_time;
	pthread_mutex_lock(&po->cmn->stdout);
	print_alive_state(po, po->new_time, "is eating");
	pthread_mutex_unlock(&po->cmn->stdout);
	check_full(po);
	while (1)
	{
		if (!po->cmn->is_surv || po->cmn->is_full)
		{
			if (!po->cmn->is_surv)
			{
				put_down(po);
				pthread_mutex_lock(&po->cmn->stdout);
				print_died_state(po);
				pthread_mutex_unlock(&po->cmn->stdout);
			}
			break ;
		}
		po->new_time = get_time();
		if (po->new_time - po->hunger_time >= po->cmn->tte)
		{
			put_down(po);
			pthread_mutex_lock(&po->cmn->stdout);
			print_alive_state(po, po->new_time, "is sleeping");
			pthread_mutex_unlock(&po->cmn->stdout);
			break ;
		}
	}
}

/*
void	get_forks(t_philo *po)
{
	int	get_fork;

	get_fork = pthread_mutex_lock(&po->cmn->forkm[po->lf]);
	if (!get_fork)
	{
		pthread_mutex_lock(&po->cmn->stdout);
		po->cmn->forks[po->lf] = 0;
		po->new_time = get_time();
		print_alive_state(po, po->new_time, "has taken a fork");
		pthread_mutex_unlock(&po->cmn->stdout);
	}
	get_fork = pthread_mutex_lock(&po->cmn->forkm[po->rf]);
	if (!get_fork)
	{
		pthread_mutex_lock(&po->cmn->stdout);
		po->cmn->forks[po->rf] = 0;
		po->new_time = get_time();
		print_alive_state(po, po->new_time, "has taken a fork");
		pthread_mutex_unlock(&po->cmn->stdout);
	}
}
*/

void	get_odd_forks(t_philo *po)
{
	int	get_fork;

	po->cmn->forks[po->rf] = 0;
	po->cmn->forks[po->lf] = 0;
	get_fork = pthread_mutex_lock(&po->cmn->forkm[po->rf]);
	if (!get_fork)
	{
		pthread_mutex_lock(&po->cmn->stdout);
		po->new_time = get_time();
		print_alive_state(po, po->new_time, "has taken a fork");
		pthread_mutex_unlock(&po->cmn->stdout);
	}
	get_fork = pthread_mutex_lock(&po->cmn->forkm[po->lf]);
	if (!get_fork)
	{
		pthread_mutex_lock(&po->cmn->stdout);
		po->new_time = get_time();
		print_alive_state(po, po->new_time, "has taken a fork");
		pthread_mutex_unlock(&po->cmn->stdout);
	}
}

void	get_even_forks(t_philo *po)
{
	int	get_fork;

	po->cmn->forks[po->lf] = 0;
	po->cmn->forks[po->rf] = 0;
	get_fork = pthread_mutex_lock(&po->cmn->forkm[po->lf]);
	if (!get_fork)
	{
		pthread_mutex_lock(&po->cmn->stdout);
		po->new_time = get_time();
		print_alive_state(po, po->new_time, "has taken a fork");
		pthread_mutex_unlock(&po->cmn->stdout);
	}
	get_fork = pthread_mutex_lock(&po->cmn->forkm[po->rf]);
	if (!get_fork)
	{
		pthread_mutex_lock(&po->cmn->stdout);
		po->new_time = get_time();
		print_alive_state(po, po->new_time, "has taken a fork");
		pthread_mutex_unlock(&po->cmn->stdout);
	}
}

void	pick_up(t_philo *po)
{
	while (1)
	{
		if (!po->cmn->is_surv || po->cmn->is_full)
		{
			if (!po->cmn->is_surv)
			{
				pthread_mutex_lock(&po->cmn->stdout);
				print_died_state(po);
				pthread_mutex_unlock(&po->cmn->stdout);
			}
			break ;
		}
		usleep(100);
		if (po->cmn->forks[po->lf] && po->cmn->forks[po->rf])
		{
			if (po->p_num % 2)
				get_odd_forks(po);
			else
				get_even_forks(po);
			break ;
		}
	}
}

int	action(t_philo *po)
{
	if (!(po->p_num % 2) && !po->eat_cnt)
	{
		while (1)
		{
			if (get_time() - po->cmn->start_time >= po->cmn->tte + 1)
				break ;
		}
	}
	pick_up(po);
	if (po->cmn->nop % 2 == 0)
	{
		
	}
	eat(po);
	do_sleep(po);
	if (po->cmn->is_full)
		return (0);
	return (po->cmn->is_surv);
}

void	*start_dining(void *info)
{
	t_philo	*po;

	po = (t_philo *)info;
	while (1)
	{
		if (po->cmn->nop == 1)
		{
			if (po->cmn->pme)
			{
				printf("0ms [1] has taken a fork\n");
				printf("%dms [1] died\n", po->cmn->ttd);
			}
			break ;
		}
		if (po->cmn->pme == 0)
			break ;
		if (!action(po))
			break ;
	}
	return (NULL);
}

void	*monitoring(void *info)
{
	t_monitor	*mnt;

	mnt = (t_monitor *)info;
	while (1)
	{
		if (!(*mnt->is_surv) || *mnt->is_full)
			break ;
		mnt->new_time = get_time();
		if (mnt->new_time - *mnt->hunger_time >= mnt->ttd)
		{
			pthread_mutex_lock(mnt->check_died);
			if (*mnt->is_surv)
			{
				*mnt->dead_time = mnt->new_time;
				*mnt->dead_p_num = mnt->m_num;
			}
			*mnt->is_surv = 0;
			pthread_mutex_unlock(mnt->check_died);
			break ;
		}
	}
	return (NULL);
}

void	create_thread(t_common *cmn, t_philo *po, t_monitor *mnt)
{
	int	i;

	cmn->is_surv = 1;
	cmn->start_time = get_time();
	i = -1;
	while (++i < cmn->nop)
	{
		pthread_create(&po[i].tid, NULL, start_dining, (void *)&po[i]);
		po[i].hunger_time = cmn->start_time;
		if (cmn->nop > 1 && (cmn->pme > 0 || cmn->pme == -1))
			pthread_create(&mnt[i].tid, NULL, monitoring, (void *)&mnt[i]);
		usleep(100);
	}
}

void	recall_resources(t_common *cmn, t_philo *po, t_monitor *mnt)
{
	int	i;

	i = -1;
	while (++i < cmn->nop)
		pthread_join(mnt[i].tid, NULL);
	i = -1;
	while (++i < cmn->nop)
		pthread_join(po[i].tid, NULL);
	pthread_mutex_destroy(&cmn->stdout);
	pthread_mutex_destroy(&cmn->check_died);
	i = -1;
	while (++i < cmn->nop)
		pthread_mutex_destroy(&cmn->forkm[i]);
	free(cmn->forkm);
	free(cmn->forks);
	free(po);
	free(mnt);
}

int	simulation(t_common *cmn)
{
	t_philo		*po;
	t_monitor	*mnt;

	mnt = (t_monitor *)malloc(sizeof(t_monitor) * cmn->nop);
	po = (t_philo *)malloc(sizeof(t_philo) * cmn->nop);
	cmn->forkm = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * cmn->nop);
	cmn->forks = (int *)malloc(sizeof(int) * cmn->nop);
	if (!mnt || !po || !cmn->forkm || !cmn->forks)
	{
		free_malloc(po, mnt, cmn->forkm, cmn->forks);
		return (print_error(1, "Malloc Error!"));
	}
	init(cmn, po, mnt);
	create_thread(cmn, po, mnt);
	recall_resources(cmn, po, mnt);
	po = NULL;
	mnt = NULL;
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
