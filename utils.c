/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 12:37:38 by tigondra          #+#    #+#             */
/*   Updated: 2026/05/26 10:25:28 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_state(t_coder *coder, char *msg)
{
	pthread_mutex_lock(&coder->simu->print_mutex);
	if (!get_stop(coder->simu))
		printf("%ld %d %s\n", timestamp(coder->simu), coder->id, msg);
	pthread_mutex_unlock(&coder->simu->print_mutex);
}

void	clean_mutex(t_simu simu)
{
	int	i;

	i = 0;
	while (i < simu.number_of_coders)
	{
		pthread_mutex_destroy(&simu.dongles[i].mutex);
		pthread_cond_destroy(&simu.dongles[i].cond);
		free(simu.dongles[i].queue.data);
		i++;
	}
	pthread_mutex_destroy(&simu.print_mutex);
	pthread_mutex_destroy(&simu.stop_mutex);
	pthread_mutex_destroy(&simu.state_mutex);
	free(simu.dongles);
	free(simu.coders);
}
