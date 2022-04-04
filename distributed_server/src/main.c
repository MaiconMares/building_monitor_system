#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <wiringPi.h>
#include "../inc/cJSON.h"
#include "../inc/device_handler.h"
#include "../inc/json_handler.h"

double central_server_port;
double distributed_server_port;
char central_server_ip[13];
char distributed_server_ip[13];

typedef struct params {
    int dht_pin;
    float temp;
    float humidity;
}params;

params dht_info;

void *dht_thread_handler(void *args){
    read_dht_data(dht_info.dht_pin, &dht_info.temp, &dht_info.humidity);
    return NULL;
}

int main(int argc, char const *argv[])
{
    wiringPiSetupGpio();
    copy_JSON_to_buffer("config_files/configuracao_andar_1.json");
    setup_server_from_JSON(
    central_server_ip, distributed_server_ip, &central_server_port, &distributed_server_port, &dht_info.dht_pin);

    pthread_t thread1;

    printf("dht_pin = %d\n", dht_info.dht_pin);

    for(;;){
        int opt = -1;
        printf("1-Ler DHT temperatura\n");
        printf("2-Sair\n");
        scanf("%d", &opt);
        if (opt == 1){
            pthread_create(&thread1, NULL, &dht_thread_handler, NULL);
            pthread_join(thread1, NULL);
        }
        else if (opt == 2)
            return 0;
        printf("Temperatura: %f\n", dht_info.temp);
        printf("Humidade: %f\n", dht_info.humidity);
    }

    return 0;
}
