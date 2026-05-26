/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 12:37:32 by tigondra          #+#    #+#             */
/*   Updated: 2026/05/26 08:24:52 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_coder	init_coder(int i, t_simu *simu)
{
	t_coder	coder;
	int		index;

	index = i - 1;
	coder.id = i;
	coder.simu = simu;
	coder.compile_count = 0;
	coder.last_compile = timestamp(simu);
	coder.left = &simu->dongles[index];
	coder.right = &simu->dongles[(index + 1) % simu->number_of_coders];
	return (coder);
}

void	init_case(t_simu *simu)
{
	int	i;

	i = 0;
	while (i < simu->number_of_coders)
	{
		simu->dongles[i] = init_dongle();
		simu->coders[i] = init_coder(i + 1, simu);
		i++;
	}
	i = 0;
	while (i < simu->number_of_coders)
	{
		simu->coders[i].last_compile = 0;
		i++;
	}
}

int	init_simu(t_simu *simu)
{
	simu->start_time = get_time_ms();
	pthread_mutex_init(&simu->print_mutex, NULL);
	simu->stop = 0;
	pthread_mutex_init(&simu->stop_mutex, NULL);
	simu->finished_coders = 0;
	pthread_mutex_init(&simu->state_mutex, NULL);
	simu->dongles = malloc(sizeof(t_dongle) * simu->number_of_coders);
	if (simu->dongles == NULL)
		return (0);
	simu->coders = malloc(sizeof(t_coder) * simu->number_of_coders);
	if (simu->coders == NULL)
	{
		free(simu->dongles);
		return (0);
	}
	init_case(simu);
	return (1);
}

void	make_simu(t_simu *simu)
{
	int	i;

	i = 0;
	pthread_create(&simu->monitor, NULL, monitor_routine, simu);
	while (i < simu->number_of_coders)
	{
		pthread_create(&simu->coders[i].thread, NULL, routine,
			&simu->coders[i]);
		i++;
	}
	i = 0;
	while (i < simu->number_of_coders)
	{
		pthread_join(simu->coders[i].thread, NULL);
		i++;
	}
	pthread_join(simu->monitor, NULL);
}
