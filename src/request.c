/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 18:11:33 by tigondra          #+#    #+#             */
/*   Updated: 2026/06/02 15:36:40 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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

int	find_request(t_simu *simu, t_coder *coder)
{
	int	i;

	i = 0;
	while (i < simu->queue.size)
	{
		if (simu->queue.data[i].coder == coder)
			return (i);
		i++;
	}
	return (-1);
}

int	request_is_before(t_request a, t_request b, int scheduler)
{
	if (scheduler == FIFO)
	{
		if (a.arrival != b.arrival)
			return (a.arrival < b.arrival);
		return (a.coder->id < b.coder->id);
	}
	if (a.deadline != b.deadline)
		return (a.deadline < b.deadline);
	return (a.coder->id < b.coder->id);
}

static int	request_has_priority(t_coder *coder, int my_index, int scheduler)
{
	t_simu		*simu;
	t_request	my_req;
	t_request	other;
	int			i;

	simu = coder->simu;
	my_req = simu->queue.data[my_index];
	i = 0;
	while (i < simu->queue.size)
	{
		other = simu->queue.data[i];
		if (i != my_index && (coder->left == other.coder->right
				|| coder->right == other.coder->left))
		{
			if (request_is_before(other, my_req, scheduler)
				&& dongles_available(other.coder))
				return (FALSE);
		}
		i++;
	}
	return (TRUE);
}

int	request_permission(t_coder *coder, int scheduler)
{
	t_request	req;
	int			index;

	req = create_request(coder);
	pthread_mutex_lock(&coder->simu->queue_mutex);
	if (!heap_push(&coder->simu->queue, req, scheduler))
		return (pthread_mutex_unlock(&coder->simu->queue_mutex), 0);
	while (!get_stop(coder->simu))
	{
		index = find_request(coder->simu, coder);
		if (index != -1 && request_has_priority(coder, index, scheduler)
			&& take_dongles(coder))
			return (check_request(coder, scheduler));
		pthread_mutex_unlock(&coder->simu->queue_mutex);
		usleep(100);
		pthread_mutex_lock(&coder->simu->queue_mutex);
	}
	index = find_request(coder->simu, coder);
	if (index != -1)
		heap_remove_at(&coder->simu->queue, index, scheduler);
	pthread_mutex_unlock(&coder->simu->queue_mutex);
	return (FALSE);
}
