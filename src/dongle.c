/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 13:41:19 by tigondra          #+#    #+#             */
/*   Updated: 2026/06/03 11:05:51 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	lock_pair(t_dongle *a, t_dongle *b)
{
	if (a < b)
	{
		pthread_mutex_lock(&a->mutex);
		pthread_mutex_lock(&b->mutex);
	}
	else
	{
		pthread_mutex_lock(&b->mutex);
		pthread_mutex_lock(&a->mutex);
	}
}

static int	reserve_one(t_coder *coder)
{
	long	now;

	now = timestamp(coder->simu);
	pthread_mutex_lock(&coder->left->mutex);
	if (!coder->left->in_use && coder->left->cooldown_until <= now)
	{
		coder->left->in_use = 1;
		pthread_mutex_unlock(&coder->left->mutex);
		print_state(coder, "has taken a dongle");
		return (TRUE);
	}
	pthread_mutex_unlock(&coder->left->mutex);
	return (FALSE);
}

int	take_dongles(t_coder *coder)
{
	long	now;

	if (coder->simu->number_of_coders == 1)
		return (reserve_one(coder));
	now = timestamp(coder->simu);
	lock_pair(coder->left, coder->right);
	if ((!coder->left->in_use && !coder->right->in_use
			&& coder->left->cooldown_until <= now
			&& coder->right->cooldown_until <= now))
	{
		coder->left->in_use = 1;
		coder->right->in_use = 1;
		pthread_mutex_unlock(&coder->left->mutex);
		pthread_mutex_unlock(&coder->right->mutex);
		print_state(coder, "has taken a dongle");
		print_state(coder, "has taken a dongle");
		return (TRUE);
	}
	pthread_mutex_unlock(&coder->left->mutex);
	pthread_mutex_unlock(&coder->right->mutex);
	return (FALSE);
}

void	drop_dongles(t_coder *coder)
{
	long	now;

	now = timestamp(coder->simu);
	pthread_mutex_lock(&coder->simu->queue_mutex);
	if (coder->simu->number_of_coders == 1)
	{
		pthread_mutex_lock(&coder->left->mutex);
		coder->left->in_use = 0;
		coder->left->cooldown_until = now + coder->simu->dongle_cooldown;
		pthread_mutex_unlock(&coder->left->mutex);
	}
	else
	{
		lock_pair(coder->left, coder->right);
		coder->left->in_use = 0;
		coder->right->in_use = 0;
		coder->left->cooldown_until = now + coder->simu->dongle_cooldown;
		coder->right->cooldown_until = now + coder->simu->dongle_cooldown;
		pthread_mutex_unlock(&coder->left->mutex);
		pthread_mutex_unlock(&coder->right->mutex);
	}
	pthread_cond_broadcast(&coder->simu->queue_cond);
	pthread_mutex_unlock(&coder->simu->queue_mutex);
}

int	dongles_available(t_coder *coder)
{
	long	now;
	int		ok;

	if (coder->simu->number_of_coders == 1)
		return (FALSE);
	now = timestamp(coder->simu);
	lock_pair(coder->left, coder->right);
	ok = (!coder->left->in_use && !coder->right->in_use
			&& coder->left->cooldown_until <= now
			&& coder->right->cooldown_until <= now);
	pthread_mutex_unlock(&coder->left->mutex);
	pthread_mutex_unlock(&coder->right->mutex);
	return (ok);
}
