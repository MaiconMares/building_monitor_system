#ifndef DEVICE_HANDLER_H
#define DEVICE_HANDLER_H

void window_handler(void);
void people_presence_T_handler(void);
void people_presence_1floor_handler(void);
void smoke_handler(void);
void main_door_handler(void);
void people_exit_from_T_handler(void);
void people_entrance_T_handler(void);
void people_exit_from_1floor_handler(void);
void people_entrance_1floor_handler(void);
int read_dht_data(int dht_pin, float *dht_temp, float *dht_humidity);

#endif
