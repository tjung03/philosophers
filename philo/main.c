#include "philosophers.h"

int	is_died(t_philo *po, long long dead_time)
{
	long long	ms_time;

	if ((dead_time - po->end_eat) > po->cmn->dt)
	{
		po->cmn->death = 1;
		ms_time = dead_time - po->cmn->start_systime;
		printf("%7lldms [%d] died\n", ms_time, po->philo_num);
		return (1);
	}
	return (0);
}

int	philo_think(t_philo *po, long long end_sleep)
{
	long long	ms_time;

	if (is_died(po, end_sleep) || po->cmn->death)
		return (1);
	ms_time = end_sleep - po->cmn->start_systime;
	printf("%7lldms [%d] is thinking\n", ms_time, po->philo_num);
	return (0);
}

int	philo_sleep(t_philo *po)
{
	long long	ms_time;
	long long	end_sleep;

	ms_time = po->end_eat - po->cmn->start_systime;
	printf("%7lldms [%d] is sleeping\n", ms_time, po->philo_num);
	while (1)
	{
		ms_time = get_time();
		if ((ms_time - po->end_eat) > po->cmn->st)
			break ;
		end_sleep = ms_time;
	}
	return (philo_think(po, end_sleep));
}

int	philo_eat(t_philo *po)
{
	long long	ms_time;

	ms_time = po->get_forks - po->cmn->start_systime;
	printf("%7lldms [%d] is eating\n", ms_time, po->philo_num);
	while (1)
	{
		ms_time = get_time();
		if ((ms_time - po->get_forks) > po->cmn->et)
			break ;
		po->end_eat = ms_time;
	}
	if (po->cmn->me != -1)
		po->me_cnt++;
	if (po->me_cnt == po->cmn->me)
		po->enough++;
	pthread_mutex_unlock(&po->cmn->arr_fork[po->lf]);
	pthread_mutex_unlock(&po->cmn->arr_fork[po->rf]);
	if (po->enough == po->cmn->pn)
		return (1);
	return (0);
}

int	try_mutex_lock(t_philo *po, int fork, int flag)
{
	long long	ms_time;
	int			success;

	success = pthread_mutex_lock(&po->cmn->arr_fork[fork]);
	if (!success)
	{
		po->get_forks = get_time();
		if (is_died(po, po->get_forks) || po->cmn->death)
			return (1);
		ms_time = po->get_forks - po->cmn->start_systime;
		printf("%7lldms [%d] has taken a fork\n", ms_time, po->philo_num);
		if (!flag)
			try_mutex_lock(po, po->rf, 1);
	}
	return (0);
}

int	get_fork(t_philo *po)
{
	if (try_mutex_lock(po, po->lf, 0))
		return (1);
	return (philo_eat(po));
}

int		behave_philo(t_philo *po)
{
	if (get_fork(po))
		return (1);
	if (po->enough)
		return (1);
	if (philo_sleep(po))
		return (1);
	return (0);
}

void	*go_dining(void *info)
{
	t_philo		*po;

	po = (t_philo *)info;
	while (1)
	{
		if (po->cmn->all_seated == 1)
		{
			if (po->cmn->pn % 2)
			{
				if (po->philo_num == 1)
					usleep(50);
				else if (!(po->philo_num % 2))
					usleep(100);
			}
			else if (!(po->cmn->pn % 2))
			{
				if (!(po->philo_num % 2))
					usleep(100);
			}
			if (behave_philo(po))
				break ;
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
	{
		pthread_join(po[i].pid, NULL);
		pthread_mutex_destroy(&po->cmn->arr_fork[i]);
	}
	free(po->cmn->arr_fork);
	free(po);
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
	init_thread_info(cmn, po);
	i = -1;
	while (++i < cmn->pn)
		pthread_create(&po[i].pid, NULL, go_dining, (void *)&po[i]);
	cmn->start_systime = get_time();
	i = -1;
	while (++i < cmn->pn)
		po[i].end_eat = cmn->start_systime;
	cmn->all_seated = 1;
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
