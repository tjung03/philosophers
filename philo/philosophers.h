#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <sys/types.h>
# include <semaphore.h>

/*
 *		pn == number_of_philosophers
 *		dt == time_to_die
 *		et == time_to_eat
 *		st == time_to_sleep
 *		me == number_of_times_each_philosopher_must_eat
 */
typedef struct s_argu {
	int	pn;
	int	dt;
	int	et;
	int	st;
	int	me;
}	t_argu;

typedef struct s_philo {
	pthread_t	pid;
	int			lf;
	int			rf;
	int			me_val;
	int			*me_cnt;
}	t_philo;

typedef struct s_global {
	struct s_argu	opt;
	struct s_philo	*philo;
	int				me_cnt;
}	t_global;

/*
 *		ft.c
 */
int	ft_atoi(char *s);

/*
 *		tools.c
 */
int	print_error(int ret, char *s);

/*
 *		parsing.c
 */
int	get_options(t_global *g, int ac, char **av);

#endif
