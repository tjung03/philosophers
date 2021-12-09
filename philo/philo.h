/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjung <tjung@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 14:28:25 by tjung             #+#    #+#             */
/*   Updated: 2021/12/09 21:49:11 by tjung            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_common {
	pthread_mutex_t	stdout;
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

/*
 *			print.c
 */
int			print_error(int ret, char *s);
void		print_alive_state(t_philo *po, long long ntime, char *s);
void		print_died_state(t_philo *po);

/*
 *			tools.c
 */
long long	get_time(void);
long long	waiting(t_philo *po, long long start, long long standard);
int			my_atoi(char *s);

/*
 *			parsing.c
 */
int			get_options(t_common *cmn, int ac, char **av);

/*
 *			recall.c
 */
void		free_malloc_by_failed(t_common *cmn, t_philo *po);
void		recall_resources(t_common *cmn, t_philo *po);

/*
 *			init.c
 */
void		init(t_common *cmn, t_philo *po);

/*
 *			thread.c
 */
void		create_thread(t_common *cmn, t_philo *po, pthread_t *mnt_tid);

/*
 *			action_dining.c
 */
int			action_dining(t_philo *po);

/*
 *			action_get_forks.c
 */
void		odd_pick_up(t_philo *po);
void		even_pick_up(t_philo *po);
void		put_down(t_philo *po);

#endif
