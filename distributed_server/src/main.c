#include <stdio.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <wiringPi.h>
#include "../inc/cJSON.h"
#include "../inc/json_handler.h"
#include "../inc/device_handler.h"
#include "../inc/TCP_communication.h"

double central_server_port;
double distributed_server_port;
char central_server_ip[13];
char distributed_server_ip[13];

typedef struct params {
    int dht_pin;
    float temp;
    float humidity;
}params;

typedef struct server_info {
    unsigned short port;
    char ip_addr[13];
    int msg_code;
} server_info;

params dht_info;
server_info central_server_info;
server_info my_server_info;

void *dht_thread_handler(void *args){
    read_dht_data(dht_info.dht_pin, &dht_info.temp, &dht_info.humidity);
    return NULL;
}

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        printf("Example of usage: make run FILENAME=config.json\n");
        printf("Provide just the filename, not the entire fullpath\n");
        return 0;
    }

    char file_path[255];
    strcpy(file_path, "config_files/");
    strcat(file_path, argv[1]);

    wiringPiSetupGpio();
    copy_JSON_to_buffer(file_path);
    setup_server_from_JSON(
    central_server_ip, distributed_server_ip, &central_server_port, &distributed_server_port, &dht_info.dht_pin);

    pthread_t thread1;
    pthread_t thread2;

    central_server_info.port = (unsigned short) central_server_port;
    strcpy(central_server_info.ip_addr, central_server_ip);

    my_server_info.port = (unsigned short) distributed_server_port;
    strcpy(my_server_info.ip_addr, distributed_server_ip);

    /*
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
    }*/

    pthread_create(&thread1, NULL, &read_from_central_server, &my_server_info);
    pthread_create(&thread2, NULL, &write_to_central_server, &central_server_info);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    return 0;
}
