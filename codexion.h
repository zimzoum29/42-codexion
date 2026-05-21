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
# include <unistd.h>
# include <stdlib.h>
# include <aio.h>
# include <sys/time.h>

typedef struct s_simu t_simu;

typedef struct s_dongle
{
    int             available;
    int             id;
    pthread_mutex_t mutex;
}                   t_dongle;

typedef struct s_coder
{
    pthread_t       thread;
    t_dongle*       left;
    t_dongle*       right;
    t_simu*         simu;
    int             id;
    int             compile_count;
    long            last_compile_start;
}                   t_coder;

typedef struct s_simu
{
    long            time;
    int             number_of_coders;
    int             time_to_burnout;
    int             time_to_compile;
    int             time_to_debug;
    int             time_to_refactor;
    int             number_of_compiles_required;
    int             dongle_cooldown;
    char*           scheduler;
    t_coder*        coders;
    t_dongle*       dongles;
    long            start_time;
    pthread_mutex_t print_mutex;
    int             stop;
    pthread_mutex_t stop_mutex;
    int             finished_coders;
    pthread_t       monitor;
    pthread_mutex_t state_mutex;
}                   t_simu;

long get_time_ms(void);
long timestamp(t_simu *simu);
int get_stop(t_simu *simu);
void set_stop(t_simu *simu);

#endif