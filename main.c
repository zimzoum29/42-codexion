/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 12:37:23 by tigondra          #+#    #+#             */
/*   Updated: 2026/05/22 14:55:31 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_simu	simu;

	if (ac == 9)
	{
		if (!ft_parse_arg(av, &simu))
		{
			printf("Error\n");
			return (1);
		}
		if (init_simu(&simu) == 1)
			return (1); 
		make_simu(&simu);
		clean_mutex(simu);
	}
	else
		printf("You must launch the program with ./simu number_of_coders \
            time_to_burnout time_to_compile time_to_debug time_to_refactor \
            number_of_compiles_required dongle_cooldown scheduler");
	return (0);
}
