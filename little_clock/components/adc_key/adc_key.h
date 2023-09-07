#ifndef _ADC_KEY_
#define _ADC_KEY_

#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   64          //Multisampling

uint32_t get_adc(void);
uint8_t get_adc_key(void);
void adc_key_init(void);

#endif // !_ADC_KEY_



