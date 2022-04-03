#include <stdio.h>
#include <wiringPi.h>
#include "../inc/cJSON.h"
#include "../inc/device_handler.h"
#include "../inc/json_handler.h"

double central_server_port;
double distributed_server_port;
char central_server_ip[13];
char distributed_server_ip[13];

int main(int argc, char const *argv[])
{
    copy_JSON_to_buffer("config_files/configuracao_andar_1.json");
    setup_server_from_JSON(
    central_server_ip, distributed_server_ip, &central_server_port, &distributed_server_port);

    return 0;
}
