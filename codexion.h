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

typedef struct s_dongle
{
    int         available;
    int         id;
}               t_dongle;

typedef struct s_coder
{
    pthread_t   thread;
    t_dongle*   dongle;
    int         id;
}               t_coder;

typedef struct s_simu
{
    long        time;
    int         number_of_coders;
    int         time_to_burnout;
    int         time_to_compile;
    int         time_to_debug;
    int         time_to_refactor;
    int         number_of_compiles_required;
    int         dongle_cooldown;
    char*       scheduler;
    t_coder*    coders;
    t_dongle*   dongles;
}               t_simu;

#endif