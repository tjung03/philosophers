#include "philosophers.h"

int	main(int ac, char **av)
{
	t_global	g;

	memset(&g, 0, sizeof(t_global));
	if (ac == 5 || ac == 6)
	{
		if (!get_options(&g, ac, av))
			return (print_error(1, "Parsing ERROR!"));
	}
	else
		return (print_error(1, "Parsing ERROR!"));
	return (0);
}
