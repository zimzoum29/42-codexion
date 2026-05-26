/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simu.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 16:25:48 by tigondra          #+#    #+#             */
/*   Updated: 2026/05/13 10:37:45 by tigondra         ###   ########.fr       */
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

typedef struct s_simu	t_simu;
typedef struct s_coder	t_coder;

typedef enum e_scheduler
{
	FIFO,
	EDF
}						t_scheduler;

typedef struct s_request
{
	t_coder				*coder;
	long				arrival;
	long				deadline;
}						t_request;

typedef struct s_heap
{
	t_request			*data;
	int					size;
	int					capacity;
}						t_heap;

typedef struct s_dongle
{
	pthread_mutex_t		mutex;
	pthread_cond_t		cond;
	t_heap				queue;
	int					owner_id;
	long				cooldown_until;
}						t_dongle;

typedef struct s_coder
{
	pthread_t			thread;
	t_dongle			*left;
	t_dongle			*right;
	t_simu				*simu;
	int					id;
	int					compile_count;
	long				last_compile;
}						t_coder;

typedef struct s_simu
{
	long				time;
	int					number_of_coders;
	int					time_to_burnout;
	int					time_to_compile;
	int					time_to_debug;
	int					time_to_refactor;
	int					number_of_compiles_required;
	int					dongle_cooldown;
	t_scheduler			scheduler;
	t_coder				*coders;
	t_dongle			*dongles;
	long				start_time;
	pthread_mutex_t		print_mutex;
	int					stop;
	pthread_mutex_t		stop_mutex;
	int					finished_coders;
	pthread_t			monitor;
	pthread_mutex_t		state_mutex;
}						t_simu;

long					get_time_ms(void);
long					timestamp(t_simu *simu);
int						get_stop(t_simu *simu);
void					set_stop(t_simu *simu);
void					take_dongles(t_coder *coder);
void					drop_dongles(t_coder *coder);
t_dongle				init_dongle(void);
void					release_dongle(t_coder *coder, t_dongle *dongle);
void					*monitor_routine(void *arg);
void					compile(t_coder *coder);
void					debug(t_coder *coder);
void					refactor(t_coder *coder);
void					*routine(void *arg);
t_coder					init_coder(int i, t_simu *simu);
int						init_simu(t_simu *simu);
void					make_simu(t_simu *simu);
void					print_state(t_coder *coder, char *msg);
int						ft_parse_arg(char **av, t_simu *simu);
void					clean_mutex(t_simu simu);
void					request_dongle(t_coder *coder, t_dongle *dongle);
int						request_is_before(t_request a, t_request b, t_scheduler scheduler);
void					heap_push(t_heap *heap, t_request req, int scheduler);
t_request				heap_top(t_heap *heap);
void					heap_pop(t_heap *heap, int scheduler);
void					heapify_down(t_heap *heap, int index, int scheduler);
void					heapify_up(t_heap *heap, int index, int scheduler);

#endif