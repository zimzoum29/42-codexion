/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 12:35:27 by tigondra          #+#    #+#             */
/*   Updated: 2026/06/03 11:09:22 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_simu	simu;

	if (ac != 9)
	{
		fprintf(stderr, "Usage: ./codexion number_of_coders time_to_burnout");
		fprintf(stderr, " time_to_compile time_to_debug time_to_refactor");
		fprintf(stderr, " number_of_compiles_required");
		fprintf(stderr, " dongle_cooldown fifo|edf\n");
		return (1);
	}
	if (!parse_args(av, &simu) || !init_simu(&simu))
		return (1);
	return (make_simu(&simu));
}
