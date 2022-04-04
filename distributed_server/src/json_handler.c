#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "../inc/json_handler.h"
#include "../inc/cJSON.h"
#include "../inc/device_handler.h"

char *buffer;

void copy_JSON_to_buffer(char *file_path){
	FILE *ptr = fopen(file_path, "r");

	if (ptr == NULL)
		printf("Error when opening the file!\n");

	fseek(ptr, 0, SEEK_END);
	long fsize = ftell(ptr);
	fseek(ptr, 0, SEEK_SET);

	buffer = malloc(fsize + 1);
	fread(buffer, fsize, 1, ptr);
	fclose(ptr);

	buffer[fsize] = '\0';
}

void setup_server_from_JSON(
    char *central_serv_ip, char *distributed_serv_ip,
    double *central_serv_port, double *distributed_serv_port, int *dht_pin){

	const cJSON *central_server_ip;
	const cJSON *central_server_port;
	const cJSON *distributed_server_ip;
	const cJSON *distributed_server_port;

	cJSON *parsed_json = cJSON_Parse(buffer);

	if (parsed_json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
    }

	central_server_ip = cJSON_GetObjectItemCaseSensitive(parsed_json, "ip_servidor_central");
	central_server_port = cJSON_GetObjectItemCaseSensitive(parsed_json, "porta_servidor_central");
	distributed_server_ip = cJSON_GetObjectItemCaseSensitive(parsed_json, "ip_servidor_distribuido");
	distributed_server_port = cJSON_GetObjectItemCaseSensitive(parsed_json, "porta_servidor_distribuido");

    memcpy(central_serv_ip, central_server_ip->valuestring, 12);
    memcpy(central_serv_port, &central_server_port->valuedouble, 8);
    memcpy(distributed_serv_ip, distributed_server_ip->valuestring, 12);
    memcpy(distributed_serv_port, &distributed_server_port->valuedouble, 8);

    central_serv_ip[12] = '\0';
    distributed_serv_ip[12] = '\0';

	cJSON *inputs;
	cJSON *outputs;
	cJSON *temperature_sensor;

	inputs = cJSON_GetObjectItemCaseSensitive(parsed_json, "inputs");
	cJSON *input;
	cJSON_ArrayForEach(input, inputs){
		cJSON *curr_gpio = cJSON_GetObjectItemCaseSensitive(input, "gpio");
		int pin_gpio = (int) curr_gpio->valuedouble;
		if (pin_gpio != 0)
			pinMode(pin_gpio, INPUT);

		if (pin_gpio == 02)
			wiringPiISR(pin_gpio, INT_EDGE_RISING, &people_entrance_1floor_handler);
		else if (pin_gpio == 03)
			wiringPiISR(pin_gpio, INT_EDGE_RISING, &people_exit_from_1floor_handler);
		else if ((pin_gpio == 5) || (pin_gpio == 6) || (pin_gpio == 9) || (pin_gpio == 11))
			wiringPiISR(pin_gpio, INT_EDGE_RISING, &window_handler);
		else if (pin_gpio == 10)
			wiringPiISR(pin_gpio, INT_EDGE_RISING, &main_door_handler);
		else if (pin_gpio == 13)
			wiringPiISR(pin_gpio, INT_EDGE_RISING, &people_entrance_T_handler);
		else if (pin_gpio == 18)
			wiringPiISR(pin_gpio, INT_EDGE_RISING, &people_presence_1floor_handler);
		else if (pin_gpio == 19)
			wiringPiISR(pin_gpio, INT_EDGE_RISING, &people_exit_from_T_handler);
		else if ((pin_gpio == 23) || (pin_gpio == 24))
			wiringPiISR(pin_gpio, INT_EDGE_RISING, &smoke_handler);
		else if (pin_gpio == 26)
			wiringPiISR(pin_gpio, INT_EDGE_RISING, &people_presence_T_handler);

	}

	outputs = cJSON_GetObjectItemCaseSensitive(parsed_json, "outputs");
	cJSON *output;
	cJSON_ArrayForEach(output, outputs){
		cJSON *curr_gpio = cJSON_GetObjectItemCaseSensitive(output, "gpio");
		int pin_gpio = (int) curr_gpio->valuedouble;
		if (pin_gpio != 0)
			pinMode(pin_gpio, OUTPUT);
	}

	temperature_sensor = cJSON_GetObjectItemCaseSensitive(parsed_json, "sensor_temperatura");
	cJSON *temp_sensor;
	cJSON_ArrayForEach(temp_sensor, temperature_sensor){
		cJSON *curr_gpio = cJSON_GetObjectItemCaseSensitive(temp_sensor, "gpio");
		int pin_gpio = (int) curr_gpio->valuedouble;
		if (pin_gpio != 0)
			memcpy(dht_pin, &pin_gpio, 4);
	}

	cJSON_Delete(parsed_json);
}

int get_device_gpio(char *array_key, char *device_tag_value){
	const cJSON *devices = NULL;
	const cJSON *device = NULL;
	int device_gpio = -1;
	char *device_tag_key = "tag";
	char *gpio_tag_key = "gpio";

	cJSON *parsed_json = cJSON_Parse(buffer);

	if (parsed_json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
    }

	devices = cJSON_GetObjectItemCaseSensitive(parsed_json, array_key);
	cJSON_ArrayForEach(device, devices){
		cJSON *tag = cJSON_GetObjectItemCaseSensitive(device, device_tag_key);

		if (!strcmp(device_tag_value, tag->valuestring)){
			cJSON *gpio = cJSON_GetObjectItemCaseSensitive(device, gpio_tag_key);
			device_gpio = (int) gpio->valuedouble;
		}
	}

	return device_gpio;
}
