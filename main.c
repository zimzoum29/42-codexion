/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 12:37:23 by tigondra          #+#    #+#             */
/*   Updated: 2026/05/26 08:19:44 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_simu	simu;

	if (ac == 9)
	{
		if (!ft_parse_arg(av, &simu) && !init_simu(&simu))
		{
			printf("Error\n");
			return (0);
		}
		make_simu(&simu);
		clean_mutex(simu);
	}
	else
		printf("You must launch the program with ./simu number_of_coders \
            time_to_burnout time_to_compile time_to_debug time_to_refactor \
            number_of_compiles_required dongle_cooldown scheduler");
	return (0);
}
