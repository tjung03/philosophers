#include "philosophers.h"

int	is_died(t_philo *po, long long dead_time, char state)
{
	pthread_mutex_lock(&po->cmn->ctrl_print);
	po->cmn->death = 1;
	print_state(po, dead_time, state);
	pthread_mutex_unlock(&po->cmn->ctrl_print);
	return (1);
}

int	philo_think(t_philo *po, long long think_time)
{
	if (po->cmn->death)
		return (1);
	usleep(100);
	pthread_mutex_lock(&po->cmn->ctrl_print);
	print_state(po, think_time, 't');
	pthread_mutex_unlock(&po->cmn->ctrl_print);
	return (0);
}

int	philo_sleep(t_philo *po, long long sleep_time)
{
	long long	ms_time;

	if (po->cmn->death)
		return (1);
	usleep(100);
	pthread_mutex_lock(&po->cmn->ctrl_print);
	print_state(po, sleep_time, 's');
	pthread_mutex_unlock(&po->cmn->ctrl_print);
	while (1)
	{
		ms_time = get_time();
		if (ms_time - po->hunger_start >= po->cmn->dt)
			return (is_died(po, ms_time, 'd'));
		else if (ms_time - sleep_time >= po->cmn->st)
			return (philo_think(po, ms_time));
	}
	return (1);
}

int	philo_enough(t_philo *po, int *cnt, int *enough)
{
	(*cnt)++;
	if (*cnt == po->cmn->me)
		po->cmn->all_enough += ++(*enough);
	if (po->cmn->pn == po->cmn->all_enough)
	{
		po->cmn->death = 1;
		return (1);
	}
	return (0);
}

int	philo_eat(t_philo *po)
{
	long long	ms_time;

	if (po->cmn->death)
	{
		pthread_mutex_unlock(&po->cmn->ctrl_print);
		return (1);
	}
	usleep(100);
	po->hunger_start = po->get_forks;
	print_state(po, po->get_forks, 'e');
	pthread_mutex_unlock(&po->cmn->ctrl_print);
	while (1)
	{
		ms_time = get_time();
		if (ms_time - po->hunger_start >= po->cmn->dt)
			return (is_died(po, ms_time, 'd'));
		else if (ms_time - po->get_forks >= po->cmn->et)
		{
			pthread_mutex_unlock(&po->cmn->arr_fork[po->lf]);
			pthread_mutex_unlock(&po->cmn->arr_fork[po->rf]);
			if (philo_enough(po, &po->me_cnt, &po->enough))
				return (1);
			return (philo_sleep(po, ms_time));
		}
	}
	return (1);
}

int	try_mutex_lock(t_philo *po, int fork, int flag)
{
	int			get_fork;
	long long	ms_time;

	if (po->cmn->death)
		return (1);
	po->try_get_fork = 1;
	get_fork = pthread_mutex_lock(&po->cmn->arr_fork[fork]);
	if (!get_fork)
	{
		po->get_forks = get_time();
		if (po->get_forks - po->hunger_start >= po->cmn->dt)
		{
			ms_time = po->hunger_start + po->cmn->dt;
			return (is_died(po, ms_time, 'd'));
		}
		usleep(100);
		pthread_mutex_lock(&po->cmn->ctrl_print);
		print_state(po, po->get_forks, 'f');
		if (!flag)
		{
			pthread_mutex_unlock(&po->cmn->ctrl_print);
			try_mutex_lock(po, po->rf, 1);
		}
	}
	po->try_get_fork = 0;
	return (0);
}

int	get_fork(t_philo *po)
{
	if (po->cmn->death)
		return (1);
	if (try_mutex_lock(po, po->lf, 0))
		return (1);
	return (philo_eat(po));
}

void	*go_dining(void *info)
{
	t_philo		*po;

	po = (t_philo *)info;
	while (1)
	{
		if (po->cmn->all_seated == 1)
		{
			if (po->cmn->pn == 1)
			{
				po->cmn->death = 1;
				printf("%dms [%d] died\n", 0, po->philo_num);
				return (NULL);
			}
			if (!po->me_cnt && !(po->philo_num % 2))
				usleep(10000);
			if (get_fork(po))
			{//
				if (po->cmn->fork_death)
					is_died(po, po->get_forks - po->hunger_start + po->cmn->start_systime, 'd');
				return (NULL);
			}//
		}
	}
	return (NULL);
}

void	init_thread_info(t_common *cmn, t_philo *po)
{
	int	i;

	memset(po, 0, sizeof(*po));
	i = -1;
	while (++i < cmn->pn)
	{
		po[i].philo_num = i + 1;
		po[i].lf = i;
		if (i == 0)
			po[i].rf = cmn->pn - 1;
		else
			po[i].rf = i - 1;
		pthread_mutex_init(&cmn->arr_fork[i], NULL);
		po[i].cmn = cmn;
	}
}

void	end_thread(t_philo *po)
{
	int	i;

	i = -1;
	while (++i < po->cmn->pn)
		pthread_join(po[i].pid, NULL);
	i = -1;
	while (++i < po->cmn->pn)
		pthread_mutex_destroy(&po->cmn->arr_fork[i]);
	free(po->cmn->arr_fork);
	pthread_mutex_destroy(&po->cmn->ctrl_print);
	free(po);
}

void	is_dead_while_get_fork(t_common *cmn, t_philo *po)
{
	long long	ms_time;
	int			i;

	while (!cmn->death)
	{
		i = -1;
		while (++i < cmn->pn)
		{
			if (po[i].try_get_fork)
			{
				ms_time = get_time();
				if (ms_time - po[i].hunger_start >= cmn->dt)
				{
					cmn->death = 1;
					cmn->fork_death = 1;
				}
			}
		}
	}
}

int	dining_philo(t_common *cmn)
{
	t_philo	*po;
	int		i;

	po = (t_philo *)malloc(sizeof(t_philo) * cmn->pn);
	if (!po)
		return (print_error(1, "Philo threads ERROR!"));
	cmn->arr_fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * cmn->pn);
	if (!(cmn->arr_fork))
		return (print_error(1, "Array fork malloc ERROR!"));
	pthread_mutex_init(&cmn->ctrl_print, NULL);
	init_thread_info(cmn, po);
	i = -1;
	while (++i < cmn->pn)
		pthread_create(&po[i].pid, NULL, go_dining, (void *)&po[i]);
	cmn->start_systime = get_time();
	i = -1;
	while (++i < cmn->pn)
		po[i].hunger_start = cmn->start_systime;
	cmn->all_seated = 1;
	is_dead_while_get_fork(cmn, po);
	end_thread(po);
	po = NULL;
	return (0);
}

int	main(int ac, char **av)
{
	t_common	cmn;

	memset(&cmn, 0, sizeof(cmn));
	if (ac == 5 || ac == 6)
	{
		if (get_options(&cmn, ac, av))
			return (print_error(1, "Parsing ERROR!(options)"));
		if (dining_philo(&cmn))
			return (print_error(1, "dining_philo() ERROR!"));
	}
	else
		return (print_error(1, "Parsing ERROR!"));
	return (0);
}
