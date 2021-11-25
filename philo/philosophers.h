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
	pthread_mutex_t	*forks;
	pthread_mutex_t	stdout_mutex;
	long long		start_time;
	int				pn;
	int				dt;
	int				et;
	int				st;
	int				me;
	int				death;
	int				first_death;
	int				all_seated;
	int				all_enough;
}	t_common;

typedef struct s_philo {
	pthread_t		pid;
	long long		hunger_start;
	long long		new_time;
	long long		base_time;
	long long		dead_time;
	int				philo_number;
	int				lf;
	int				rf;
	int				eat_cnt;
	int				enough;
	struct s_common	*cmn;
}	t_philo;

typedef struct	s_monitor {
	pthread_t		pid;
	pthread_mutex_t	*stdout_mutex;
	long long		*start_time;
	long long		*hunger_start;
	long long		dead_time;
	long long		new_time;
	int				*dead_philo_number;
	int				*death;
	int				*first_death;
	int				*all_seated;
	int				dt;
}	t_monitor;

/*
 *			ft.c
 */
int			ft_atoi(char *s);

/*
 *			tools.c
 */
int			print_error(int ret, char *s);
void		print_alive_state(t_philo *po, long long ntime, char state);
long long	get_time(void);

/*
 *			parsing.c
 */
int			get_options(t_common *g, int ac, char **av);

/*
 *			init.c
 */
t_philo		*init_philo_data(t_common *cmn);
t_monitor	*init_monitor_data(t_common *cmn, t_philo *po);

/*
 *			monitor.c
 */
void		*monitoring(void *info);

 /*
 *			dining.c
 */
void		*dining(void *info);

/*
 *			action.c
 */
int			get_forks(t_philo *po, int fork, int flag);
int			philo_eat(t_philo *po);
int			philo_sleep(t_philo *po);
int			philo_think(t_philo *po);

 /*
 *			died.c
 */
void		print_dead_state(t_monitor *mnt);
void		*died_one_philo(t_philo *po);
void		died_philo(t_monitor *mnt);

#endif
