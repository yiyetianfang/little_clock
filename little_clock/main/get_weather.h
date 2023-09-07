#ifndef _GET_WEATHER_H
#define _GET_WEATHER_H
/*
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include "esp_system.h"

#include <string.h>
#include "freertos/task.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_tls.h"
#include "esp_http_client.h"

#include "cJSON.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "lwip/sockets.h"
*/
#include "main.h"


extern char datestr[3][20];
extern char tempstr[3][20];
extern char humiditystr[3][20];
extern char Rainfallstr[3][20];

extern uint8_t get_flag;

void get_weather_app(void);

#endif

