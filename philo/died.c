#include "philosophers.h"

void	print_dead_state(t_monitor *mnt)
{
	long long	ms_time;

	ms_time = mnt->dead_time - *mnt->start_time;
	if (!(*mnt->first_death))
	{
		*mnt->first_death = 1;
		printf("%lldms [%d] died\n", ms_time, *mnt->dead_philo_number);
	}
}

void	*died_one_philo(t_philo *po)
{
	po->cmn->death = 1;
	printf("%dms [%d] died\n", 0, po->philo_number);
	return (NULL);
}

void	died_philo(t_monitor *mnt)
{
	*mnt->death = 1;
	pthread_mutex_lock(mnt->stdout_mutex);
	print_dead_state(mnt);
	pthread_mutex_unlock(mnt->stdout_mutex);
}
