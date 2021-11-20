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

int	get_options(t_common *cmn, int ac, char **av)
{
	if (is_valid(ac, av))
		return (print_error(1, "It is not valid!"));
	cmn->pn = ft_atoi(av[1]);
	cmn->dt = ft_atoi(av[2]);
	cmn->et = ft_atoi(av[3]);
	cmn->st = ft_atoi(av[4]);
	cmn->me = -1;
	if (ac == 6)
	{
		cmn->me = ft_atoi(av[5]);
		if (cmn->me < 0)
			return (print_error(1, "It must be a value greater than zero!"));
	}
	if (cmn->pn >= 200
		|| cmn->dt < 60 || cmn->et < 60 || cmn->st < 60)
		return (print_error(1, "This value doesn't meet the conditions!"));
	return (0);
}
