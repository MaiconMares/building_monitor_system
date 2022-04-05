#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <wiringPi.h>
#include "../inc/queue.h"
#include "../inc/device_handler.h"

// CONSTANTS
#define MAX_TIMINGS	85
#define DEBUG 0
#define WAIT_TIME 2000
#define EXIT_FROM_T 	 10
#define ENTRANCE_T 		 20
#define EXIT_FROM_1FLOOR 30
#define ENTRANCE_1FLOOR  40

// GLOBAL VARIABLES
char mode = 'c';

int data[5] = { 0, 0, 0, 0, 0 };
float temp_cels = -1;
float temp_fahr = -1;
float humidity  = -1;

typedef struct server_info {
    unsigned short port;
    char ip_addr[13];
    int msg_code;
} server_info;

void window_handler(void){
    printf("Janela aberta!\n");
}

void people_presence_T_handler(void){
    printf("(Térreo): Presença de pessoa detectada\n");
}

void people_presence_1floor_handler(void){
    printf("(1º Andar): Presença de pessoa detectada\n");
}

void smoke_handler(void){
    printf("Fumaça detectada\n");
}

void main_door_handler(void){
    printf("(Térreo): Abertura de porta\n");
}

void people_exit_from_T_handler(void){
	insert(EXIT_FROM_T);

    printf("(Térreo): Pessoa saindo do térreo\n");
}

void people_entrance_T_handler(void){
	insert(ENTRANCE_T);

    printf("(Térreo): Pessoa entrando no térreo\n");
}

void people_exit_from_1floor_handler(void){
	insert(EXIT_FROM_1FLOOR);

    printf("(1º Andar): Pessoa saindo do Primeiro Andar\n");
}

void people_entrance_1floor_handler(void){
	insert(ENTRANCE_1FLOOR);

    printf("(1º Andar): Pessoa entrando no Primeiro Andar\n");
}

int read_dht_data(int dht_pin, float *dht_temp, float *dht_humidity) {
	uint8_t laststate = HIGH;
	uint8_t counter	= 0;
	uint8_t j = 0;
	uint8_t i;

	data[0] = data[1] = data[2] = data[3] = data[4] = 0;

	/* pull pin down for 18 milliseconds */
	pinMode(dht_pin, OUTPUT);
	digitalWrite(dht_pin, LOW);
	delay(18);

	/* prepare to read the pin */
	pinMode(dht_pin, INPUT);

    printf("ok\n");
	/* detect change and read data */
	for ( i = 0; i < MAX_TIMINGS; i++ ) {
		counter = 0;
		while ( digitalRead( dht_pin ) == laststate ) {
			counter++;
			delayMicroseconds( 1 );
			if ( counter == 255 ) {
				break;
			}
		}
		laststate = digitalRead( dht_pin );

		if ( counter == 255 )
			break;

		/* ignore first 3 transitions */
		if ( (i >= 4) && (i % 2 == 0) ) {
			/* shove each bit into the storage bytes */
			data[j / 8] <<= 1;
			if ( counter > 16 )
				data[j / 8] |= 1;
			j++;
            printf("j = %d\n", j);
		}
	}

	if ( (j >= 40) && (data[4] == ( (data[0] + data[1] + data[2] + data[3]) & 0xFF) ) ) {
		float h = (float)((data[0] << 8) + data[1]) / 10;
		if ( h > 100 ) {
			h = data[0];	// for DHT11
		}
		float c = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
		if ( c > 125 ) {
			c = data[2];	// for DHT11
		}
		if ( data[2] & 0x80 ) {
			c = -c;
		}
		temp_cels = c;
		temp_fahr = c * 1.8f + 32;
		humidity = h;

        printf("%f\n", temp_cels);

        memcpy(dht_humidity, &humidity, 4);
        memcpy(dht_temp, &temp_cels, 4);

		if (DEBUG) printf( "read_dht_data() Humidity = %.1f %% Temperature = %.1f *C (%.1f *F)\n", humidity, temp_cels, temp_fahr );
		return 0; // OK
	} else {
		if (DEBUG) printf( "read_dht_data() Data not good, skip\n" );
		temp_cels = temp_fahr = humidity = -1;
		return 1; // NOK
	}
}
