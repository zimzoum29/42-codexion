/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 12:28:58 by tigondra          #+#    #+#             */
/*   Updated: 2026/06/02 15:34:11 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	wait_threads_init(t_coder *coder)
{
	while (1)
	{
		pthread_mutex_lock(&coder->simu->scheduler_mutex);
		if (coder->simu->state == 1)
		{
			pthread_mutex_unlock(&coder->simu->scheduler_mutex);
			break ;
		}
		if (coder->simu->state == 2)
		{
			pthread_mutex_unlock(&coder->simu->scheduler_mutex);
			return (FALSE);
		}
		pthread_mutex_unlock(&coder->simu->scheduler_mutex);
		usleep(100);
	}
	return (TRUE);
}

int	has_finished(t_coder *coder)
{
	int	ok;

	pthread_mutex_lock(&coder->simu->state_mutex);
	ok = coder->compile_count >= coder->simu->number_of_compiles_required;
	pthread_mutex_unlock(&coder->simu->state_mutex);
	return (ok);
}

int	check_request(t_coder *coder, int scheduler)
{
	int	index;

	index = find_request(coder->simu, coder);
	heap_remove_at(&coder->simu->queue, index, scheduler);
	pthread_cond_broadcast(&coder->simu->queue_cond);
	pthread_mutex_unlock(&coder->simu->queue_mutex);
	return (1);
}

int	request_compile_permission(t_coder *coder)
{
	if (coder->simu->scheduler == FIFO)
		return (request_permission(coder, FIFO));
	return (request_permission(coder, EDF));
}
