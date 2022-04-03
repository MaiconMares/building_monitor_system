#ifndef JSON_HANDLER_H
#define JSON_HANDLER_H

//Copy JSON to a char *buffer in order to avoid constant accesses to memory
//It should called before all JSON functions
void copy_JSON_to_buffer(char *file_path);

//Setup pins and register addresses for servers
void setup_server_from_JSON(
    char *central_serv_ip, char *distributed_serv_ip,
    double *central_serv_port, double *distributed_serv_port);

int get_device_gpio(char *array_key, char *device_tag_value);

#endif
