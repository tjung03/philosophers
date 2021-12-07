#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

# define TRUE 1

typedef struct s_common {
	pthread_mutex_t	stdout;
	pthread_mutex_t	check_died;
	pthread_mutex_t	*forkm;
	int				*forks;
	long long		start_time;
	long long		dead_time;
	int				dead_p_num;
	int				first_death;
	int				nop;
	int				ttd;
	int				tte;
	int				tts;
	int				pme;
	int				is_surv;
	int				is_full;
	int				full_cnt;
	int				is_seat;
}	t_common;

typedef struct s_philo {
	struct s_common	*cmn;
	pthread_t		tid;
	long long		hunger_time;
	long long		new_time;
	int				p_num;
	int				lf;
	int				rf;
	int				eat_cnt;
	int				full;
}	t_philo;

typedef struct	s_monitor {
	pthread_t		tid;
	pthread_mutex_t	*check_died;
	long long		*hunger_time;
	long long		*dead_time;
	long long		new_time;
	int				*dead_p_num;
	int				*is_surv;
	int				*is_full;
	int				*is_seat;
	int				ttd;
	int				m_num;
}	t_monitor;

/*
 *			tools.c
 */
void		print_alive_state(t_philo *po, long long ntime, char *s);
void		print_died_state(t_philo *po);
long long	get_time(void);
int			print_error(int ret, char *s);
int			my_atoi(char *s);

/*
 *			parsing.c
 */
int			get_options(t_common *cmn, int ac, char **av);

#endif
