/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 12:37:38 by tigondra          #+#    #+#             */
/*   Updated: 2026/05/22 01:55:11 by tigondra         ###   ########.fr       */
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

int	is_number(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	valid_scheduler(char *str)
{
	if (strcmp(str, "fifo") == 0)
		return (1);
	if (strcmp(str, "edf") == 0)
		return (1);
	return (0);
}

int	ft_parse_arg(char **av, t_simu *simu)
{
	int	i;

	i = 1;
	while (i <= 7)
	{
		if (!is_number(av[i]))
			return (0);
		i++;
	}
	if (!valid_scheduler(av[8]))
		return (0);
	simu->number_of_coders = atoi(av[1]);
	simu->time_to_burnout = atoi(av[2]);
	simu->time_to_compile = atoi(av[3]);
	simu->time_to_debug = atoi(av[4]);
	simu->time_to_refactor = atoi(av[5]);
	simu->number_of_compiles_required = atoi(av[6]);
	simu->dongle_cooldown = atoi(av[7]);
	if (strcmp(av[8], "fifo") == 0)
		simu->scheduler = FIFO;
	else
		simu->scheduler = EDF;
	if (simu->number_of_coders <= 0)
		return (0);
	return (1);
}
