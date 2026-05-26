/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 12:37:20 by tigondra          #+#    #+#             */
/*   Updated: 2026/05/26 08:56:00 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	wait_cooldown(t_coder *coder, t_dongle *dongle)
{
	while (!get_stop(coder->simu)
		&& timestamp(coder->simu) < dongle->cooldown_until)
		usleep(500);
}

void	take_dongles(t_coder *coder)
{
	if (coder->simu->number_of_coders == 1)
	{
		request_dongle(coder, coder->left);
		return ;
	}
	if (coder->left < coder->right)
	{
		request_dongle(coder, coder->left);
		request_dongle(coder, coder->right);
	}
	else
	{
		request_dongle(coder, coder->right);
		request_dongle(coder, coder->left);
	}
}

void	drop_dongles(t_coder *coder)
{
	release_dongle(coder, coder->left);
	release_dongle(coder, coder->right);
}

t_dongle	init_dongle(void)
{
	t_dongle	dongle;

	pthread_mutex_init(&dongle.mutex, NULL);
	pthread_cond_init(&dongle.cond, NULL);
	dongle.queue.data = NULL;
	dongle.queue.size = 0;
	dongle.queue.capacity = 0;
	dongle.owner_id = 0;
	dongle.cooldown_until = 0;
	return (dongle);
}

void	release_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->owner_id = 0;
	dongle->cooldown_until = timestamp(coder->simu)
		+ coder->simu->dongle_cooldown;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}