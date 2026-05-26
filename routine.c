/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 12:37:28 by tigondra          #+#    #+#             */
/*   Updated: 2026/05/26 09:08:06 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	compile(t_coder *coder)
{
	take_dongles(coder);
	if (coder->simu->number_of_coders == 1)
	{
		usleep(coder->simu->time_to_burnout * 1000);
		set_stop(coder->simu);
		return ;
	}
	pthread_mutex_lock(&coder->simu->state_mutex);
	coder->last_compile = timestamp(coder->simu);
	pthread_mutex_unlock(&coder->simu->state_mutex);
	print_state(coder, "is compiling");
	coder->compile_count++;
	if (coder->compile_count == coder->simu->number_of_compiles_required)
	{
		pthread_mutex_lock(&coder->simu->stop_mutex);
		coder->simu->finished_coders++;
		if (coder->simu->finished_coders >= coder->simu->number_of_coders)
			coder->simu->stop = 1;
		pthread_mutex_unlock(&coder->simu->stop_mutex);
	}
	usleep(coder->simu->time_to_compile * 1000);
	drop_dongles(coder);
}

void	debug(t_coder *coder)
{
	print_state(coder, "is debugging");
	usleep(coder->simu->time_to_debug * 1000);
}

void	refactor(t_coder *coder)
{
	print_state(coder, "is refactoring");
	usleep(coder->simu->time_to_refactor * 1000);
}

void	*routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!get_stop(coder->simu))
	{
		compile(coder);
		if (get_stop(coder->simu))
			break ;
		debug(coder);
		if (get_stop(coder->simu))
			break ;
		refactor(coder);
	}
	return (NULL);
}
