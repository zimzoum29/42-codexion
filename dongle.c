/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 12:37:20 by tigondra          #+#    #+#             */
/*   Updated: 2026/05/22 01:52:15 by tigondra         ###   ########.fr       */
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
		pthread_mutex_lock(&coder->left->mutex);
		print_state(coder, "has taken a dongle");
		return ;
	}
	if (coder->id % 2 == 0)
	{
		pthread_mutex_lock(&coder->right->mutex);
		wait_cooldown(coder, coder->right);
		print_state(coder, "has taken a dongle");
		pthread_mutex_lock(&coder->left->mutex);
		wait_cooldown(coder, coder->left);
		print_state(coder, "has taken a dongle");
	}
	else
	{
		pthread_mutex_lock(&coder->left->mutex);
		wait_cooldown(coder, coder->left);
		print_state(coder, "has taken a dongle");
		pthread_mutex_lock(&coder->right->mutex);
		wait_cooldown(coder, coder->right);
		print_state(coder, "has taken a dongle");
	}
}

void	drop_dongles(t_coder *coder)
{
	coder->left->cooldown_until = timestamp(coder->simu)
		+ coder->simu->dongle_cooldown;
	coder->right->cooldown_until = timestamp(coder->simu)
		+ coder->simu->dongle_cooldown;
	pthread_mutex_unlock(&coder->left->mutex);
	pthread_mutex_unlock(&coder->right->mutex);
}

t_dongle	init_dongle()
{
	t_dongle	dongle;

	pthread_mutex_init(&dongle.mutex, NULL);
	dongle.cooldown_until = 0;
	return (dongle);
}
