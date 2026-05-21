#include "codexion.h"



void print_state(t_coder *coder, char *msg)
{
    pthread_mutex_lock(&coder->simu->print_mutex);
    if (!get_stop(coder->simu))
        printf("%ld %d %s\n", timestamp(coder->simu), coder->id, msg);
    pthread_mutex_unlock(&coder->simu->print_mutex);
}

void take_dongles(t_coder *coder)
{
    if (coder->simu->number_of_coders == 1)
    {
        pthread_mutex_lock(&coder->left->mutex);
        print_state(coder, "has taken a dongle");
        return ;
    }
    else if (coder->id % 2 == 0)
    {
        pthread_mutex_lock(&coder->right->mutex);
        print_state(coder, "has taken a dongle");
        pthread_mutex_lock(&coder->left->mutex);
        print_state(coder, "has taken a dongle");
    }
    else
    {
        pthread_mutex_lock(&coder->left->mutex);
        print_state(coder, "has taken a dongle");
        pthread_mutex_lock(&coder->right->mutex);
        print_state(coder, "has taken a dongle");
    }
}

void drop_dongles(t_coder *coder)
{
    pthread_mutex_unlock(&coder->left->mutex);
    pthread_mutex_unlock(&coder->right->mutex);
}

void *monitor_routine(void *arg)
{
    t_simu *simu;
    int    i;
    long   now;

    simu = (t_simu *)arg;
    while (!get_stop(simu))
    {
        i = 0;
        while (i < simu->number_of_coders)
        {
            now = timestamp(simu);
            pthread_mutex_lock(&simu->state_mutex);
            if (now - simu->coders[i].last_compile_start > simu->time_to_burnout)
            {
                pthread_mutex_unlock(&simu->state_mutex);
                pthread_mutex_lock(&simu->print_mutex);
                printf("%ld %d burned out\n", now, simu->coders[i].id);
                pthread_mutex_unlock(&simu->print_mutex);
                set_stop(simu);
                return (NULL);
            }
            pthread_mutex_unlock(&simu->state_mutex);
            i++;
        }
        usleep(1000);
    }
    return (NULL);
}

void compile(t_coder *coder)
{
    take_dongles(coder);
    if (coder->simu->number_of_coders == 1)
    {
        usleep(coder->simu->time_to_burnout * 1000);
        pthread_mutex_unlock(&coder->left->mutex);
        set_stop(coder->simu);
        return ;
    }
    pthread_mutex_lock(&coder->simu->state_mutex);
    coder->last_compile_start = timestamp(coder->simu);
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

void debug(t_coder *coder)
{
    print_state(coder, "is debugging");
    usleep(coder->simu->time_to_debug * 1000);
}

void refactor(t_coder *coder)
{
    print_state(coder, "is refactoring");
    usleep(coder->simu->time_to_refactor * 1000);
}

void *routine(void *arg)
{
    t_coder *coder;

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

t_dongle init_dongle(int i)
{
    t_dongle dongle;

    dongle.available = 1;
    dongle.id = i;
    pthread_mutex_init(&dongle.mutex, NULL);
    return (dongle);
}

t_coder init_coder(int i, t_simu *simu)
{
    t_coder coder;
    int     index;

    index = i - 1;
    coder.id = i;
    coder.simu = simu;
    coder.compile_count = 0;
    coder.last_compile_start = 0;
    coder.left = &simu->dongles[index];
    coder.right = &simu->dongles[(index + 1) % simu->number_of_coders];
    return (coder);
}

void    init_simu(t_simu* simu)
{
    int i;

    i = 0;
    simu->start_time = get_time_ms();
    pthread_mutex_init(&simu->print_mutex, NULL);
    simu->stop = 0;
    pthread_mutex_init(&simu->stop_mutex, NULL);
    simu->finished_coders = 0;
    pthread_mutex_init(&simu->state_mutex, NULL);
    simu->dongles = malloc(sizeof(t_dongle) * simu->number_of_coders);
    simu->coders = malloc(sizeof(t_coder) * simu->number_of_coders);
    while (i < simu->number_of_coders)
    {
        simu->dongles[i] = init_dongle(i);
        simu->coders[i] = init_coder(i + 1, simu);
        i++;
    }
    i = 0;
    while (i < simu->number_of_coders)
    {
        simu->coders[i].last_compile_start = 0;
        i++;
    }
}

void make_simu(t_simu *simu)
{
    int i;

    i = 0;
    pthread_create(&simu->monitor, NULL, monitor_routine, simu);
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
    pthread_join(simu->monitor, NULL);
}

int main(int ac, char** av)
{
    t_simu simu;
    int i;

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
        printf("You must launch the program with ./simu number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler");
    return 0;
}