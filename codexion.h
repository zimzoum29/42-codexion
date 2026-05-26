/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 12:36:04 by tigondra          #+#    #+#             */
/*   Updated: 2026/05/26 20:00:00 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_simu		t_simu;
typedef struct s_coder		t_coder;

typedef enum e_scheduler
{
	FIFO,
	EDF
}	t_scheduler;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	long			cooldown_until;
	int				in_use;
}	t_dongle;

typedef struct s_request
{
	t_coder			*coder;
	long			arrival;
	long			deadline;
}	t_request;

typedef struct s_heap
{
	t_request		*data;
	int				size;
	int				capacity;
}	t_heap;

typedef struct s_coder
{
	pthread_t		thread;
	t_dongle		*left;
	t_dongle		*right;
	t_simu			*simu;
	int				id;
	int				compile_count;
	long			last_compile;
}	t_coder;

typedef struct s_simu
{
	int				number_of_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	t_scheduler		scheduler;
	t_coder			*coders;
	t_dongle		*dongles;
	long			start_time;
	int				stop;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	state_mutex;
	pthread_t		monitor_thread;
	t_heap			queue;
	pthread_mutex_t	queue_mutex;
	pthread_cond_t	queue_cond;
}	t_simu;

int			parse_args(char **av, t_simu *simu);
int			init_simu(t_simu *simu);
int			make_simu(t_simu *simu);
void		clean_simu(t_simu *simu);
void		*routine(void *arg);
void		*monitor_routine(void *arg);
long		get_time_ms(void);
long		timestamp(t_simu *simu);
void		ms_sleep(t_simu *simu, long time);
int			get_stop(t_simu *simu);
void		set_stop(t_simu *simu);
void		stop_and_wake(t_simu *simu);
void		print_state(t_coder *coder, char *msg);
void		print_burnout(t_simu *simu, int id, long now);
int			request_compile_permission(t_coder *coder);
int			take_dongles(t_coder *coder);
void		drop_dongles(t_coder *coder);
int			request_is_before(t_request a, t_request b, int scheduler);
int			heap_push(t_heap *heap, t_request request, int scheduler);
t_request	*heap_top(t_heap *heap);
void		heap_pop(t_heap *heap, int scheduler);

#endif
