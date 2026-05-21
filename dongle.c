#include "codexion.h"

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

t_dongle init_dongle(int i)
{
    t_dongle dongle;

    dongle.available = 1;
    dongle.id = i;
    pthread_mutex_init(&dongle.mutex, NULL);
    return (dongle);
}
