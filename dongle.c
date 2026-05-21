/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 12:37:20 by tigondra          #+#    #+#             */
/*   Updated: 2026/05/21 14:12:02 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	take_dongles(t_coder *coder)
{
	if (coder->simu->number_of_coders == 1)
	{
		pthread_mutex_lock(&coder->left->mutex);
		print_state(coder, "has taken a dongle");
		return ;
	}
    pthread_mutex_lock(&coder->left->mutex);
    print_state(coder, "has taken a dongle");
    pthread_mutex_lock(&coder->right->mutex);
    print_state(coder, "has taken a dongle");
}

void	drop_dongles(t_coder *coder)
{
	pthread_mutex_unlock(&coder->left->mutex);
	pthread_mutex_unlock(&coder->right->mutex);
}

t_dongle	init_dongle(int i)
{
	t_dongle	dongle;

	pthread_mutex_init(&dongle.mutex, NULL);
	return (dongle);
}
