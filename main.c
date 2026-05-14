#include "codexion.h"

void   compile(int coder_id, int time)
{
    printf("%d is compiling\n", coder_id);
    sleep(time);
}

void   debug(int coder_id, int time)
{
    printf("%d is debuging\n", coder_id);
    sleep(time);
}

void   refactor(int coder_id, int time)
{
    printf("%d is refactoring\n", coder_id);
    sleep(time);
}

void*    routine(void* coder)
{
    t_coder* coder1;

    coder1 = (t_coder*)coder;
    compile(coder1->id, 1);
    debug(coder1->id, 1);
    refactor(coder1->id, 1);
    pthread_exit(NULL);
}

void  ft_parse_arg(int ac, char** av, t_simu* simu)
{
    simu->time = 0;
    simu->number_of_coders = atoi(av[1]);
    // simu->time_to_burnout = atoi(av[2]);
    // simu->time_to_compile = atoi(av[3]);
    // simu->time_to_debug = atoi(av[4]);
    // simu->time_to_refactor = atoi(av[5]);
    // simu->number_of_compiles_required = atoi(av[6]);
    // simu->dongle_cooldown = atoi(av[7]);
    // simu->scheduler = av[8];
}

t_dongle    init_dongle(int i)
{
    t_dongle    dongle;

    dongle.available = 1;
    dongle.id = i;
    return dongle;
}

t_coder     init_coder(int i, t_dongle* dongle)
{
    t_coder     coder;
    pthread_t   thread;

    coder.dongle = dongle;
    coder.id = i;
    coder.thread = thread;
    return coder; 
}

void    init_simu(t_simu* simu)
{
    int i;

    i = 0;
    simu->dongles = malloc(sizeof(t_dongle) * simu->number_of_coders);
    simu->coders = malloc(sizeof(t_coder) * simu->number_of_coders);
    while (i < simu->number_of_coders)
    {
        simu->dongles[i] = init_dongle(i);
        simu->coders[i] = init_coder(i, &simu->dongles[i]);
        i++;
    }   
}

void    make_simu(t_simu* simu)
{
        int i = 0;
        while (i < simu->number_of_coders)
        { 
            pthread_create(&simu->coders[i].thread, NULL, routine, &simu->coders[i]);
            i++;
        }
        i = 0;
        while (i < simu->number_of_coders)
        {
            pthread_join(simu->coders[i].thread, NULL);
            i++;
        }
}

int main(int ac, char** av)
{
    t_simu simu;
    if (ac == 2)
    {
        ft_parse_arg(ac, av, &simu);
        init_simu(&simu);
        make_simu(&simu);
    }
    else
        printf("You must launch the program with ./simu number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler");
    return 0;
}