/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 12:28:58 by tigondra          #+#    #+#             */
/*   Updated: 2026/06/02 15:13:55 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int    wait_threads_init(t_coder *coder)
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
	int	finished;

	pthread_mutex_lock(&coder->simu->state_mutex);
	finished = (coder->compile_count
			>= coder->simu->number_of_compiles_required);
	pthread_mutex_unlock(&coder->simu->state_mutex);
	return (finished);
}