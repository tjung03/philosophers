/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjung <tjung@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 14:28:25 by tjung             #+#    #+#             */
/*   Updated: 2021/12/11 00:50:18 by tjung            ###   ########.fr       */
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
	long long		start_time;
	int				nop;
	int				ttd;
	int				tte;
	int				tts;
	int				pme;
	int				is_surv;
	int				full_cnt;
}	t_common;

typedef struct s_philo {
	struct s_common	*cmn;
	pthread_t		tid;
	long long		hunger_time;
	int				p_num;
	int				lf;
	int				rf;
	int				eat_cnt;
	int				full;
}	t_philo;

/*
 *			tools.c
 */
int			print_error(int ret, char *s);
long long	get_time(void);
int			my_atoi(char *s);

/*
 *			parsing.c
 */
int			get_options(t_common *cmn, int ac, char **av);

/*
 *			recall.c
 */
void		failed_free(t_philo *po, pthread_t *mnt, pthread_mutex_t *forkm);
void		recall_resources(t_common *cmn, t_philo *po, pthread_t *mnt);

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
void		pick_up(t_philo *po);
void		eat(t_philo *po);
void		do_sleep(t_philo *po);
void		think(t_philo *po);

#endif
