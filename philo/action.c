#include "philosophers.h"

int	philo_think(t_philo *po)
{
	pthread_mutex_lock(&po->cmn->stdout_mutex);
	print_alive_state(po, po->new_time, 't');
	pthread_mutex_unlock(&po->cmn->stdout_mutex);
	if (po->cmn->death)
		return (1);
	return (0);
}

int	philo_sleep(t_philo *po)
{
	po->base_time = po->new_time;
	pthread_mutex_lock(&po->cmn->stdout_mutex);
	print_alive_state(po, po->base_time, 's');
	pthread_mutex_unlock(&po->cmn->stdout_mutex);
	while (1)
	{
		if (po->cmn->death)
			return (1);
		po->new_time = get_time();
		if (po->new_time - po->base_time >= po->cmn->st)
			break ;
	}
	return (0);
}

int	philo_eat(t_philo *po)
{
	po->base_time = po->new_time;
	pthread_mutex_lock(&po->cmn->stdout_mutex);
	print_alive_state(po, po->base_time, 'e');
	pthread_mutex_unlock(&po->cmn->stdout_mutex);
	po->hunger_start = po->new_time;
	po->eat_cnt++;
	if (po->eat_cnt == po->cmn->me)
		po->cmn->all_enough += ++(po->enough);
	while (1)
	{
		if (po->cmn->death)
		{
			pthread_mutex_unlock(&po->cmn->forks[po->lf]);
			pthread_mutex_unlock(&po->cmn->forks[po->rf]);
			return (1);
		}
		po->new_time = get_time();
		if (po->new_time - po->base_time >= po->cmn->et)
		{
			pthread_mutex_unlock(&po->cmn->forks[po->lf]);
			pthread_mutex_unlock(&po->cmn->forks[po->rf]);
			break ;
		}
	}
	return (0);
}

int	get_forks(t_philo *po, int fork, int flag)
{
	int			get_fork;

	get_fork = pthread_mutex_lock(&po->cmn->forks[fork]);
	if (!get_fork)
	{
		if (po->cmn->death)
		{
			pthread_mutex_unlock(&po->cmn->forks[po->lf]);
			if (flag == 1)
				pthread_mutex_unlock(&po->cmn->forks[po->rf]);
			return (1);
		}
		po->new_time = get_time();
		usleep(100);
		pthread_mutex_lock(&po->cmn->stdout_mutex);
		print_alive_state(po, po->new_time, 'f');
		pthread_mutex_unlock(&po->cmn->stdout_mutex);
		if (!flag)
			get_forks(po, po->rf, 1);
	}
	return (0);
}
