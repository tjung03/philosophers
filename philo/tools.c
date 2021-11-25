#include "philosophers.h"

int			print_error(int ret, char *s)
{
	if (ret == 1)
		printf("%s\n", s);
	return (ret);
}

void		print_alive_state(t_philo *po, long long ntime, char state)
{
	long long	ms_time;

	ms_time = ntime - po->cmn->start_time;
	if (!po->cmn->death)
	{
		if (po->cmn->pn != po->cmn->all_enough)
		{
			printf("%lldms [%d] ", ms_time, po->philo_number);
			if (state == 'f')
				printf("has taken a fork\n");
			else if (state == 'e')
				printf("is eating\n");
			else if (state == 's')
				printf("is sleeping\n");
			else if (state == 't')
				printf("is thinking\n");
		}
	}
}

long long	get_time(void)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((long long)(now.tv_sec * 1000 + now.tv_usec / 1000));
}
