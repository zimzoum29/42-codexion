/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 15:07:07 by tigondra          #+#    #+#             */
/*   Updated: 2026/05/30 14:29:47 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	coder_burned_out(t_simu *simu, int i, long *now)
{
	int	burned;

	*now = timestamp(simu);
	pthread_mutex_lock(&simu->state_mutex);
	burned = (*now - simu->coders[i].last_compile > simu->time_to_burnout);
	pthread_mutex_unlock(&simu->state_mutex);
	return (burned);
}

void	*monitor_routine(void *arg)
{
	t_simu	*simu;
	int		i;
	long	now;

	simu = (t_simu *)arg;
	if (simu->state == 2)
		return (NULL);
	while (!get_stop(simu))
	{
		i = 0;
		while (i < simu->number_of_coders && !get_stop(simu))
		{
			if (coder_burned_out(simu, i, &now))
			{
				print_burnout(simu, simu->coders[i].id, now);
				stop_and_wake(simu);
				return (NULL);
			}
			i++;
		}
		usleep(100);
	}
	return (NULL);
}
 