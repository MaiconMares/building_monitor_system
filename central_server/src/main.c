#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "../inc/TCP_communication.h"

int main(int argc, char const *argv[])
{
    pthread_t thread1;
    pthread_t thread2;

    pthread_create(&thread1, NULL, &read_from_distributed_server, NULL);
    pthread_create(&thread2, NULL, &write_to_distributed_server, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
