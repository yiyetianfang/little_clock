/* ADC1 Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "adc_key.h"



static esp_adc_cal_characteristics_t *adc_chars;

static const adc_channel_t channel = ADC_CHANNEL_5;     //GPIO33 if ADC1, GPIO14 if ADC2
static const adc_bits_width_t width = ADC_WIDTH_BIT_12;

static const adc_atten_t atten = ADC_ATTEN_DB_11;  /*!<The input voltage of ADC will be attenuated extending the range of measurement by about 11 dB (3.55 x) */
static const adc_unit_t unit = ADC_UNIT_1;

uint16_t key_Voltage[][3] =
    {
        {0, 0, 342},   //keyn,Voltage-min,Voltage-max
        {1, 1529,1929},
        {2, 2095, 2495},

};
//key1: Raw: 0\tVoltage: 142mV
//key2: Raw: 1983\tVoltage:1729mV
//key3: Raw: 2689\tVoltage:2295mv



static void check_efuse(void)
{

    //Check if TP is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
        printf("eFuse Two Point: Supported\n");
    } else {
        printf("eFuse Two Point: NOT supported\n");
    }
    //Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK) {
        printf("eFuse Vref: Supported\n");
    } else {
        printf("eFuse Vref: NOT supported\n");
    }
}


static void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        printf("Characterized using Two Point Value\n");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        printf("Characterized using eFuse Vref\n");
    } else {
        printf("Characterized using Default Vref\n");
    }
}


uint32_t get_adc(void)
{
        uint32_t adc_reading = 0;
        //Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            if (unit == ADC_UNIT_1) {
                adc_reading += adc1_get_raw((adc1_channel_t)channel);
            } else {
                int raw;
                adc2_get_raw((adc2_channel_t)channel, width, &raw);
                adc_reading += raw;
            }
        }
        adc_reading /= NO_OF_SAMPLES;
        //Convert adc_reading to voltage in mV
        uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
       // printf("Raw: %d\tVoltage: %dmV\n", adc_reading, voltage);
        return voltage;
}

uint8_t get_adc_key(void)
{
    uint32_t adc_vol;
    adc_vol = get_adc();
    for(int i = 0; i < 3; i++) 
    {
        if(adc_vol >= key_Voltage[i][1]&&adc_vol <= key_Voltage[i][2])
        {
            return i + 1;
            printf("Voltage: %dmV\n",adc_vol);
        }
    }
    return 0;

}

void adc_key_init(void)
{
        //Check if Two Point or Vref are burned into eFuse
    check_efuse();

    //Configure ADC
    if (unit == ADC_UNIT_1) {
        adc1_config_width(width);
        adc1_config_channel_atten(channel, atten);
    } else {
        adc2_config_channel_atten((adc2_channel_t)channel, atten);
    }

    //Characterize ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, width, DEFAULT_VREF, adc_chars);
    print_char_val_type(val_type);
}

/*
void app_main(void)
{

    adc_key_init();
    //Continuously sample ADC1
    while (1) {
        
        uint8_t keyn;
        keyn = get_adc_key();
         printf("key: %d\n", keyn);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
*/
