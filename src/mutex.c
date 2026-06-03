/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 13:45:18 by tigondra          #+#    #+#             */
/*   Updated: 2026/06/03 14:16:41 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	cleanup_mutex(t_simu *simu, int index)
{
	if (index >= 1 || index == 0)
		pthread_mutex_destroy(&simu->print_mutex);
	if (index >= 2 || index == 0)
		pthread_mutex_destroy(&simu->stop_mutex);
	if (index >= 3 || index == 0)
		pthread_mutex_destroy(&simu->state_mutex);
	if (index >= 3 || index == 0)
		pthread_mutex_destroy(&simu->scheduler_mutex);
	if (index >= 4 || index == 0)
		pthread_cond_destroy(&simu->queue_cond);
	return (0);
}

int	init_mutex(t_simu *simu)
{
	if (pthread_mutex_init(&simu->print_mutex, NULL))
		return (0);
	if (pthread_mutex_init(&simu->stop_mutex, NULL))
		return (cleanup_mutex(simu, 1));
	if (pthread_mutex_init(&simu->state_mutex, NULL))
		return (cleanup_mutex(simu, 2));
	if (pthread_mutex_init(&simu->queue_mutex, NULL))
		return (cleanup_mutex(simu, 3));
	if (pthread_mutex_init(&simu->scheduler_mutex, NULL))
		return (cleanup_mutex(simu, 4));
	if (pthread_cond_init(&simu->queue_cond, NULL))
		return (cleanup_mutex(simu, 5));
	return (0);
}
