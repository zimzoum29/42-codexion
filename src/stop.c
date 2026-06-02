/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 08:28:23 by tigondra          #+#    #+#             */
/*   Updated: 2026/05/27 08:28:42 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	get_stop(t_simu *simu)
{
	int	stop;

	pthread_mutex_lock(&simu->stop_mutex);
	stop = simu->stop;
	pthread_mutex_unlock(&simu->stop_mutex);
	return (stop);
}

void	set_stop(t_simu *simu)
{
	pthread_mutex_lock(&simu->stop_mutex);
	simu->stop = 1;
	pthread_mutex_unlock(&simu->stop_mutex);
}

void	stop_and_wake(t_simu *simu)
{
	set_stop(simu);
	pthread_mutex_lock(&simu->queue_mutex);
	pthread_cond_broadcast(&simu->queue_cond);
	pthread_mutex_unlock(&simu->queue_mutex);
}
