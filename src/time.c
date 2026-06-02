/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 14:00:09 by tigondra          #+#    #+#             */
/*   Updated: 2026/05/27 08:28:31 by tigondra         ###   ########.fr       */
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
