#include "philosophers.h"

int	start_dining(t_philo *info)
{

}

void	init_thread_info(t_global *g)
{
	int	i;

	memset(g->philo, 0, sizeof(*(g->philo)));
	i = -1;
	while (++i < g->opt.pn)
	{
		g->philo[i].me_val = g->opt.me;
		g->philo[i].me_cnt = &g->me_cnt;
	}
}

int	dining_philo(t_global *g)
{
	int	i;

	g->philo = (t_philo *)malloc(sizeof(t_philo) * g->opt.pn);
	if (!(g->philo))
		return (print_error(1, "Philo threads ERROR!"));
	init_thread_info(g);
	i = -1;
	while (++i < g->opt.pn)
	{
		pthread_create(g->philo->pid, NULL, start_dining, &g->philo[i]);
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_global	g;

	memset(&g, 0, sizeof(g));
	if (ac == 5 || ac == 6)
	{
		if (get_options(&g, ac, av))
			return (print_error(1, "Parsing ERROR!(options)"));
		if (dining_philo(&g))
			return (print_error(1, "dining_philo() ERROR!"));
	}
	else
		return (print_error(1, "Parsing ERROR!"));
	return (0);
}
