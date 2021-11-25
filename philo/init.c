#include "philosophers.h"

t_philo		*init_philo_data(t_common *cmn)
{
	t_philo	*po;
	int		i;

	po = (t_philo *)malloc(sizeof(t_philo) * cmn->pn);
	cmn->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * cmn->pn);
	if (!po || !cmn->forks)
	{
		printf("Malloc Error!(philo_data)\n");
		return (NULL);
	}
	pthread_mutex_init(&cmn->stdout_mutex, NULL);
	memset(po, 0, sizeof(*po));
	i = -1;
	while (++i < cmn->pn)
	{
		po[i].philo_number = i + 1;
		po[i].lf = i;
		if (i == 0)
			po[i].rf = cmn->pn - 1;
		else
			po[i].rf = i - 1;
		pthread_mutex_init(&cmn->forks[i], NULL);
		po[i].cmn = cmn;
	}
	return (po);
}

t_monitor	*init_monitor_data(t_common *cmn, t_philo *po)
{
	t_monitor	*mnt;
	int			i;

	mnt = (t_monitor *)malloc(sizeof(t_monitor) * cmn->pn);
	if (!mnt)
	{
		printf("Malloc Error!(monitor_data)\n");
		return (NULL);
	}
	memset(mnt, 0, sizeof(*mnt));
	i = -1;
	while (++i < cmn->pn)
	{
		mnt[i].stdout_mutex = &cmn->stdout_mutex;
		mnt[i].start_time = &cmn->start_time;
		mnt[i].hunger_start = &po[i].hunger_start;
		mnt[i].dead_philo_number = &po[i].philo_number;
		mnt[i].death = &cmn->death;
		mnt[i].first_death = &cmn->first_death;
		mnt[i].all_seated = &cmn->all_seated;
		mnt[i].dt = cmn->dt;
	}
	return (mnt);
}
