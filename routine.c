/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 13:32:16 by tigondra          #+#    #+#             */
/*   Updated: 2026/05/29 18:50:49 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	compiles_are_completed(t_simu *simu)
{
	int	i;

	i = 0;
	while (i < simu->number_of_coders)
	{
		if (simu->coders[i].compile_count < simu->number_of_compiles_required)
			return (0);
		i++;
	}
	return (1);
}

static void	compile(t_coder *coder)
{
	int	completed;

	pthread_mutex_lock(&coder->simu->state_mutex);
	coder->last_compile = timestamp(coder->simu);
	coder->compile_count++;
	completed = compiles_are_completed(coder->simu);
	pthread_mutex_unlock(&coder->simu->state_mutex);
	print_state(coder, "is compiling");
	if (completed)
		stop_and_wake(coder->simu);
	ms_sleep(coder->simu, coder->simu->time_to_compile);
}

static void	debug(t_coder *coder)
{
	print_state(coder, "is debugging");
	ms_sleep(coder->simu, coder->simu->time_to_debug);
}

static void	refactor(t_coder *coder)
{
	print_state(coder, "is refactoring");
	ms_sleep(coder->simu, coder->simu->time_to_refactor);
}

void	*routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->id % 2 == 0)
		usleep(1000);
	if (coder->simu->number_of_coders == 1)
		return (single_coder_routine(coder));
	while (!get_stop(coder->simu))
	{
		pthread_mutex_lock(&coder->simu->state_mutex);
		if (coder->compile_count >= coder->simu->number_of_compiles_required)
		{
			pthread_mutex_unlock(&coder->simu->state_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&coder->simu->state_mutex);
		if (!request_compile_permission(coder))
			return (NULL);
		compile(coder);
		drop_dongles(coder);
		debug(coder);
		refactor(coder);
	}
	return (NULL);
}
