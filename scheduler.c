/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 18:11:33 by tigondra          #+#    #+#             */
/*   Updated: 2026/05/27 07:58:48 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	request_is_before(t_request a, t_request b, int scheduler)
{
	if (scheduler == FIFO)
	{
		if (a.arrival == b.arrival)
			return (a.coder->id < b.coder->id);
		return (a.arrival < b.arrival);
	}
	if (a.deadline == b.deadline)
		return (a.coder->id < b.coder->id);
	return (a.deadline < b.deadline);
}

static t_request	create_request(t_coder *coder)
{
	t_request	req;

	req.coder = coder;
	req.arrival = timestamp(coder->simu);
	pthread_mutex_lock(&coder->simu->state_mutex);
	req.deadline = coder->last_compile + coder->simu->time_to_burnout;
	pthread_mutex_unlock(&coder->simu->state_mutex);
	return (req);
}

static int	can_take_turn(t_coder *coder)
{
	int	i;

	i = 0;
	while (i < coder->simu->queue.size)
	{
		if (coder->simu->queue.data[i].coder == coder)
		{
			if (!take_dongles(coder))
				return (0);
			heap_remove_at(&coder->simu->queue, i, coder->simu->scheduler);
			pthread_cond_broadcast(&coder->simu->queue_cond);
			return (1);
		}
		i++;
	}
	return (0);
}

int	request_compile_permission(t_coder *coder)
{
	t_request	req;

	req = create_request(coder);
	pthread_mutex_lock(&coder->simu->queue_mutex);
	if (!heap_push(&coder->simu->queue, req, coder->simu->scheduler))
		return (pthread_mutex_unlock(&coder->simu->queue_mutex), 0);
	while (!get_stop(coder->simu))
	{
		if (can_take_turn(coder))
			return (pthread_mutex_unlock(&coder->simu->queue_mutex), 1);
		pthread_mutex_unlock(&coder->simu->queue_mutex);
		usleep(100);
		pthread_mutex_lock(&coder->simu->queue_mutex);
	}
	pthread_mutex_unlock(&coder->simu->queue_mutex);
	return (0);
}
