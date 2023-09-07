#ifndef _MAIN_H
#define _MAIN_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "lvgl.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "lvgl_helpers.h"
#include "examples/lv_examples.h"
#include "adc_key.h"
#include "examples/porting/lv_port_indev_template.h"

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

extern uint8_t keynun;
/**********************
 *  STATIC PROTOTYPES
 **********************/
//static void lv_tick_task(void *arg);
//static void guiTask(void *pvParameter);
//static void create_demo_application(void);
 void button_task(void *arg);



#endif


