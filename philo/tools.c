#include "philosophers.h"

int	print_error(int ret, char *s)
{
	if (ret == 1)
		printf("%s\n", s);
	return (ret);
}

int	print_state(t_philo *po, long long ntime, char state)
{
	long long	ms_time;

	ms_time = ntime - po->cmn->start_systime;
	if (!po->cmn->death)
	{
		printf("%lldms [%d] ", get_time() - po->cmn->start_systime, po->philo_num);//
//		printf("%lldms [%d] ", ms_time, po->philo_num);
		if (state == 'f' || state == 'F')
			printf("has taken a fork\n");
		else if (state == 'e' || state == 'E')
			printf("is eating\n");
		else if (state == 's' || state == 'S')
			printf("is sleeping\n");
		else if (state == 't' || state == 'T')
			printf("is thinking\n");
	}
	else if (!po->cmn->first_death)
	{
		po->cmn->first_death = 1;
		if (state == 'd' || state == 'D')
			printf("%lldms [%d] died\n", get_time() - po->cmn->start_systime, po->philo_num);//
//			printf("%lldms [%d] died\n", ms_time, po->philo_num);
	}
	return (0);
}

long long	get_time(void)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((long long)(now.tv_sec * 1000 + now.tv_usec / 1000));
}
