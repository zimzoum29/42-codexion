/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tigondra <tigondra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 12:37:23 by tigondra          #+#    #+#             */
/*   Updated: 2026/05/21 12:45:26 by tigondra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_simu	simu;
	int		i;

	i = 0;
	if (ac == 9)
	{
		ft_parse_arg(av, &simu);
		init_simu(&simu);
		make_simu(&simu);
		while (i < simu.number_of_coders)
		{
			pthread_mutex_destroy(&simu.dongles[i].mutex);
			i++;
		}
		pthread_mutex_destroy(&simu.print_mutex);
		pthread_mutex_destroy(&simu.stop_mutex);
		pthread_mutex_destroy(&simu.state_mutex);
		free(simu.dongles);
		free(simu.coders);
	}
	else
		printf("You must launch the program with ./simu number_of_coders \
            time_to_burnout time_to_compile time_to_debug time_to_refactor \
            number_of_compiles_required dongle_cooldown scheduler");
	return (0);
}
