/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 14:00:09 by tigondra          #+#    #+#             */
/*   Updated: 2026/05/26 20:00:00 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	v;

	gettimeofday(&v, NULL);
	return (v.tv_sec * 1000 + v.tv_usec / 1000);
}

long	timestamp(t_simu *simu)
{
	return (get_time_ms() - simu->start_time);
}

void	ms_sleep(t_simu *simu, long time)
{
	long	end;

	end = timestamp(simu) + time;
	while (!get_stop(simu) && timestamp(simu) < end)
		usleep(100);
}

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
