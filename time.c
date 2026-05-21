/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 12:37:35 by tigondra          #+#    #+#             */
/*   Updated: 2026/05/21 12:42:11 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

long	timestamp(t_simu *simu)
{
	return (get_time_ms() - simu->start_time);
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
