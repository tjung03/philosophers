#include "philosophers.h"

int	philo_think()
{
	printf("is thinking\n");
	return (0);
}

int	philo_sleep()
{
	printf("is sleeping\n");
	return (0);
}

int	philo_eat()
{
	printf("is eating\n");
	return (0);
}

int	get_fork()
{
	return (0);
}

void	*go_dining(void *info)
{
	t_philo	*po;

	po = (t_philo *)info;
/*
	philo_eat();
	philo_sleep();
	philo_think();
*/
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
		po[i].cmn = cmn;
	}
}

void	join_thread(t_philo *po)
{
	int	i;

	i = -1;
	while (++i < po->cmn->pn)
		pthread_join(po[i].pid, NULL);
}

int	dining_philo(t_common *cmn)
{
	t_philo	*po;
	int		i;

	po = (t_philo *)malloc(sizeof(t_philo) * cmn->pn);
	if (!po)
		return (print_error(1, "Philo threads ERROR!"));
	cmn->start_systime = get_time();
	cmn->arr_fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * cmn->pn);
	if (!(cmn->arr_fork))
		return (print_error(1, "Array fork malloc ERROR!"));
	init_thread_info(cmn, po);
	i = -1;
	while (++i < cmn->pn)
	{
		pthread_create(&po[i].pid, NULL, go_dining, (void *)&po[i]);
	}
	join_thread(po);
	free(po);
	free(cmn->arr_fork);
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
