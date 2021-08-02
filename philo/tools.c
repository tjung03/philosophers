#include "philosophers.h"

int	print_error(int ret, char *s)
{
	if (ret == 1)
		printf("%s\n", s);
	return (ret);
}
