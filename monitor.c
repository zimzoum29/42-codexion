/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 12:51:56 by tigondra          #+#    #+#             */
/*   Updated: 2026/05/21 12:54:35 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_burnout(t_simu *simu, int i)
{
	long	now;
	int		id;

	now = timestamp(simu);
	pthread_mutex_lock(&simu->state_mutex);
	if (now - simu->coders[i].last_compile > simu->time_to_burnout)
	{
		id = simu->coders[i].id;
		pthread_mutex_unlock(&simu->state_mutex);
		pthread_mutex_lock(&simu->print_mutex);
		printf("%ld %d burned out\n", now, id);
		pthread_mutex_unlock(&simu->print_mutex);
		set_stop(simu);
		return (1);
	}
	pthread_mutex_unlock(&simu->state_mutex);
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_simu	*simu;
	int		i;

	simu = (t_simu *)arg;
	while (!get_stop(simu))
	{
		i = 0;
		while (i < simu->number_of_coders)
		{
			if (check_burnout(simu, i))
				return (NULL);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
