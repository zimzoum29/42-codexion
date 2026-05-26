/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 01:53:50 by tigondra          #+#    #+#             */
/*   Updated: 2026/05/26 10:32:37 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	request_dongle(t_coder *coder, t_dongle *dongle)
{
	t_request	req;

	pthread_mutex_lock(&dongle->mutex);
	req.coder = coder;
	req.arrival = timestamp(coder->simu);
	req.deadline = coder->last_compile + coder->simu->time_to_burnout;
	heap_push(&dongle->queue, req, coder->simu->scheduler);
	while (!get_stop(coder->simu)
		&& (heap_top(&dongle->queue).coder != coder
			|| timestamp(coder->simu) < dongle->cooldown_until
			|| dongle->owner_id != 0))
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	if (get_stop(coder->simu))
	{
		pthread_mutex_unlock(&dongle->mutex);
		return ;
	}
	heap_pop(&dongle->queue, coder->simu->scheduler);
	dongle->owner_id = coder->id;
	pthread_mutex_unlock(&dongle->mutex);
	print_state(coder, "has taken a dongle");
}

int	request_is_before(t_request a, t_request b, t_scheduler scheduler)
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
