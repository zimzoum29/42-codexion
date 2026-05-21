#include "codexion.h"

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
