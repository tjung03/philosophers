#include "philosophers.h"

static void	recall_resources(t_common *cmn, t_philo *po, t_monitor *mnt)
{
	int	i;

	i = -1;
	while (++i < cmn->pn)
		pthread_join(mnt[i].pid, NULL);
	i = -1;
	while (++i < cmn->pn)
		pthread_join(po[i].pid, NULL);
	i = -1;
	while (++i < cmn->pn)
		pthread_mutex_destroy(&cmn->forks[i]);
	free(cmn->forks);
	pthread_mutex_destroy(&cmn->stdout_mutex);
	free(mnt);
	free(po);
	mnt = NULL;
	po = NULL;
}

static int	is_all_set(t_common *cmn, t_philo *po)
{
	int	i;

	i = -1;
	cmn->start_time = get_time();
	while (++i < cmn->pn)
		po[i].hunger_start = cmn->start_time;
	return (1);
}

static void	create_thread(t_common *cmn, t_philo *po, t_monitor *mnt)
{
	int	i;

	i = -1;
	while (++i < cmn->pn)
	{
		pthread_create(&po[i].pid, NULL, dining, (void *)&po[i]);
		pthread_create(&mnt[i].pid, NULL, monitoring, (void *)&mnt[i]);
	}
}

static int	dining_philo(t_common *cmn)
{
	t_philo		*po;
	t_monitor	*mnt;

	po = init_philo_data(cmn);
	mnt = init_monitor_data(cmn, po);
	if (!po || !mnt)
		return (print_error(1, "Failed Init!"));
	create_thread(cmn, po, mnt);
	cmn->all_seated = is_all_set(cmn, po);
	recall_resources(cmn, po, mnt);
	return (0);
}

int			main(int ac, char **av)
{
	t_common	cmn;

	memset(&cmn, 0, sizeof(cmn));
	if (ac == 5 || ac == 6)
	{
		if (get_options(&cmn, ac, av))
			return (print_error(1, "Parsing Error!(options)"));
		if (dining_philo(&cmn))
			return (print_error(1, "dining_philo() Error!"));
	}
	else
		return (print_error(1, "Parsing Error!"));
	return (0);
}
