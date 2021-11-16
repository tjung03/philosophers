#include "philosophers.h"

static int	is_valid(int ac, char **av)
{
	char	*s;
	int		i;
	int		j;

	i = 1;
	while (--ac)
	{
		s = av[i];
		j = -1;
		while (s[++j])
		{
			if (s[j] < 48 || s[j] > 57)
				return (print_error(1, "It is not numbers!"));
		}
		i++;
	}
	return (0);
}

int	get_options(t_global *g, int ac, char **av)
{
	if (is_valid(ac, av))
		return (print_error(1, "It is not valid!"));
	g->opt.pn = ft_atoi(av[1]);
	g->opt.dt = ft_atoi(av[2]);
	g->opt.et = ft_atoi(av[3]);
	g->opt.st = ft_atoi(av[4]);
	g->opt.me = -1;
	if (ac == 6)
		g->opt.me = ft_atoi(av[5]);
	if (g->opt.pn >= 200
		|| g->opt.dt < 60 || g->opt.et < 60 || g->opt.st < 60)
		return (print_error(1, "This value doesn't meet the conditions!"));
	return (0);
}
