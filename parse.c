/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 12:43:35 by tigondra          #+#    #+#             */
/*   Updated: 2026/05/26 20:00:00 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_number(char *str)
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

static int	valid_number(char *str, int can_be_zero)
{
	long	value;

	if (!is_number(str) || strlen(str) > 10)
		return (0);
	value = atoi(str);
	if (can_be_zero && value >= 0)
		return (1);
	if (!can_be_zero && value > 0)
		return (1);
	return (0);
}

static int	valid_scheduler(char *str)
{
	if (strcmp(str, "fifo") == 0)
		return (1);
	if (strcmp(str, "edf") == 0)
		return (1);
	return (0);
}

static void	fill_simu(char **av, t_simu *simu)
{
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
}

int	parse_args(char **av, t_simu *simu)
{
	int	i;

	i = 1;
	while (i <= 6)
	{
		if (!valid_number(av[i], 0))
			return (fprintf(stderr, "Invalid numeric argument\n"), 0);
		i++;
	}
	if (!valid_number(av[7], 1))
		return (fprintf(stderr, "Invalid cooldown\n"), 0);
	if (!valid_scheduler(av[8]))
		return (fprintf(stderr, "Scheduler must be fifo or edf\n"), 0);
	fill_simu(av, simu);
	return (1);
}
