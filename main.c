#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void*   compile(void* arg)
{
    while (1)
    {
        sleep(1);
        printf("Compiling\n");
    }
    return NULL;
}

void   debug()
{
    while (1)
    {
        sleep(2);
        printf("Debug\n");
    }
}

int main(void)
{
    pthread_t thread;
    pthread_create(&thread, NULL, compile, NULL);
    debug();
}