#include "philo.h"

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
	cmn->nop = my_atoi(av[1]);
	cmn->ttd = my_atoi(av[2]);
	cmn->tte = my_atoi(av[3]);
	cmn->tts = my_atoi(av[4]);
	cmn->pme = -1;
	if (ac == 6)
	{
		cmn->pme = my_atoi(av[5]);
		if (cmn->pme < 0)
			return (print_error(1, "It must be a value greater than zero!"));
	}
	if (cmn->nop < 1)
		return (print_error(1, "No philosopher!"));
	if (cmn->nop >= 200 || cmn->ttd < 60 || cmn->tte < 60 || cmn->tts < 60)
		return (print_error(1, "This value doesn't meet the conditions!"));
	return (0);
}
