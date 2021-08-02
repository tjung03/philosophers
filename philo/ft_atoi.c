#include "philosophers.h"

int	ft_atoi(char *s)
{
	int	num;
	int	i;

	num = 0;
	i = 0;
	while (s[i])
	{
		num = (num * 10) + (s[i] - 48);
		i++;
	}
	return (num);
}
