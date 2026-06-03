/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 13:36:00 by tigondra          #+#    #+#             */
/*   Updated: 2026/06/03 13:54:50 by tigondra         ###   ########.fr       */
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

void	print_burnout(t_simu *simu, int id, long now)
{
	pthread_mutex_lock(&simu->print_mutex);
	if (!get_stop(simu))
		printf("%ld %d burned out\n", now, id);
	pthread_mutex_unlock(&simu->print_mutex);
}

static void	destroy_mutexes(t_simu *simu)
{
	int	i;

	i = 0;
	while (i < simu->number_of_coders)
	{
		pthread_mutex_destroy(&simu->dongles[i].mutex);
		i++;
	}
	cleanup_mutex(simu, 0);
}

void	clean_simu(t_simu *simu)
{
	destroy_mutexes(simu);
	free(simu->queue.data);
	free(simu->dongles);
	free(simu->coders);
}

void	*single_coder_routine(t_coder *coder)
{
	if (take_dongles(coder))
	{
		while (!get_stop(coder->simu))
			usleep(100);
		drop_dongles(coder);
	}
	return (NULL);
}
