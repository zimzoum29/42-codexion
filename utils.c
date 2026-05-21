#include "codexion.h"


void print_state(t_coder *coder, char *msg)
{
    pthread_mutex_lock(&coder->simu->print_mutex);
    if (!get_stop(coder->simu))
        printf("%ld %d %s\n", timestamp(coder->simu), coder->id, msg);
    pthread_mutex_unlock(&coder->simu->print_mutex);
}

void  ft_parse_arg(char** av, t_simu* simu)
{
    simu->time = 0;
    simu->number_of_coders = atoi(av[1]);
    simu->time_to_burnout = atoi(av[2]);
    simu->time_to_compile = atoi(av[3]);
    simu->time_to_debug = atoi(av[4]);
    simu->time_to_refactor = atoi(av[5]);
    simu->number_of_compiles_required = atoi(av[6]);
    simu->dongle_cooldown = atoi(av[7]);
    simu->scheduler = av[8];
}
