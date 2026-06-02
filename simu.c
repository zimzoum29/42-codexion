/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 13:17:11 by tigondra          #+#    #+#             */
/*   Updated: 2026/05/30 14:15:37 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_dongle	init_dongle(void)
{
	t_dongle	dongle;

	pthread_mutex_init(&dongle.mutex, NULL);
	dongle.cooldown_until = 0;
	dongle.in_use = 0;
	return (dongle);
}

static t_coder	init_coder(int id, t_simu *simu)
{
	t_coder	coder;
	int		index;

	index = id - 1;
	coder.id = id;
	coder.simu = simu;
	coder.compile_count = 0;
	coder.last_compile = -1;
	coder.left = &simu->dongles[index];
	coder.right = &simu->dongles[(index + 1) % simu->number_of_coders];
	return (coder);
}

static int	init_memory(t_simu *simu)
{
	simu->dongles = malloc(sizeof(t_dongle) * simu->number_of_coders);
	if (!simu->dongles)
		return (0);
	simu->coders = malloc(sizeof(t_coder) * simu->number_of_coders);
	if (!simu->coders)
		return (free(simu->dongles), 0);
	simu->queue.data = malloc(sizeof(t_request) * simu->number_of_coders);
	if (!simu->queue.data)
	{
		free(simu->dongles);
		free(simu->coders);
		return (0);
	}
	return (1);
}

int	init_simu(t_simu *simu)
{
	int	i;

	i = 0;
	simu->start_time = get_time_ms();
	simu->stop = 0;
	simu->queue.size = 0;
	simu->state = 0;
	simu->queue.capacity = simu->number_of_coders;
	if (!init_memory(simu))
		return (0);
	pthread_mutex_init(&simu->print_mutex, NULL);
	pthread_mutex_init(&simu->stop_mutex, NULL);
	pthread_mutex_init(&simu->state_mutex, NULL);
	pthread_mutex_init(&simu->queue_mutex, NULL);
	pthread_cond_init(&simu->queue_cond, NULL);
	while (i < simu->number_of_coders)
	{
		simu->dongles[i] = init_dongle();
		simu->coders[i] = init_coder(i + 1, simu);
		i++;
	}
	return (1);
}

int	make_simu(t_simu *simu)
{
	int	i;

	i = 0;
	while (i < simu->number_of_coders)
	{
		if (pthread_create(&simu->coders[i].thread, NULL, routine,
			&simu->coders[i]) != 0)
			break ;
		i++;
	}
	pthread_mutex_lock(&simu->scheduler_mutex);
	if (i == simu->number_of_coders)
		simu->state = 1;
	else
		simu->state = 2;
	pthread_mutex_unlock(&simu->scheduler_mutex);
	pthread_create(&simu->monitor_thread, NULL, monitor_routine, simu);
	i = 0;
	while (i < simu->number_of_coders)
		pthread_join(simu->coders[i++].thread, NULL);
	pthread_join(simu->monitor_thread, NULL);
	return (1);
}
