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
				return (0);
		}
		i++;
	}
	return (1);
}

int			get_options(t_global *g, int ac, char **av)
{
	if (!is_valid(ac, av))
		return (0);
	(g->opt).pn = ft_atoi(av[1]);
	(g->opt).dt = ft_atoi(av[2]);
	(g->opt).et = ft_atoi(av[3]);
	(g->opt).st = ft_atoi(av[4]);
	(g->opt).me = -1;
	if (ac == 6)
		(g->opt).me = ft_atoi(av[5]);
	return (1);
}
