#include "philosophers.h"

static int	start_dinner(t_philo *po)
{
	if (get_forks(po, po->lf, 0))
		return (1);
	if (philo_eat(po))
		return (1);
	if (po->cmn->pn == po->cmn->all_enough)
		return (1);
	if (philo_sleep(po))
		return (1);
	if (philo_think(po))
		return (1);
	return (0);
}

void		*dining(void *info)
{
	t_philo	*po;

	po = (t_philo *)info;
	while (!po->cmn->death)
	{
		if (po->cmn->all_seated)
		{
			if (po->cmn->pn == 1)
				return (died_one_philo(po));
			if (!po->eat_cnt && !(po->philo_number % 2))
				usleep(15000);
			if (start_dinner(po))
				break ;
		}
	}
	return (NULL);
}
