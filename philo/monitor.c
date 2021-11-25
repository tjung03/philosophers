#include "philosophers.h"

void	*monitoring(void *info)
{
	t_monitor	*mnt;

	mnt = (t_monitor *)info;
	while (!(*mnt->death))
	{
		if (*mnt->all_seated)
		{
			mnt->new_time = get_time();
			if (mnt->new_time - *mnt->hunger_start >= mnt->dt)
			{
				mnt->dead_time = mnt->new_time;
				*mnt->death = 1;
				died_philo(mnt);
			}
		}
	}
	return (NULL);
}
