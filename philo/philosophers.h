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
typedef struct s_common {
	pthread_mutex_t	*arr_fork;
	pthread_mutex_t	ctrl_print;
	long long		start_systime;
	int				pn;
	int				dt;
	int				et;
	int				st;
	int				me;
	int				death;
	int				all_seated;
	int				all_enough;
}	t_common;

typedef struct s_philo {
	pthread_t		pid;
	long long		hunger_start;
	long long		get_forks;
	long long		renewal_time;
	int				philo_num;
	int				lf;
	int				rf;
	int				me_cnt;
	int				enough;
	struct s_common	*cmn;
}	t_philo;

/*
 *		ft.c
 */
int			ft_atoi(char *s);

/*
 *		tools.c
 */
int			print_error(int ret, char *s);
int			print_state(t_philo *po, long long ntime, char state);
long long	get_time(void);

/*
 *		parsing.c
 */
int			get_options(t_common *g, int ac, char **av);

#endif
