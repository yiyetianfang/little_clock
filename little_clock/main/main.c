#include "main.h"
#include "wifi_station.h"
#include "get_weather.h"
#include "display_app.h"
/*********************
 *      DEFINES
 *********************/

uint8_t keynun = 0;

/**********************
 *   APPLICATION MAIN
 **********************/
void app_main(void)
{
    // xTaskCreate(guiTask, "gui", 4096*2, NULL, 1, NULL);
    // xTaskCreatePinnedToCore(guiTask, "gui", 4096*2, NULL, 1, NULL, 1);
    /* NOTE: When not using Wi-Fi nor Bluetooth you can pin the guiTask to core 0 */
   xTaskCreatePinnedToCore(guiTask, "gui", 4096*2, NULL, 0, NULL, 1);
   xTaskCreatePinnedToCore(&button_task, "button_task", 1024 * 2, NULL, 18, NULL, 0);
   wifi_app();
   get_weather_app();
}
 


 void button_task(void *arg)
{
	adc_key_init();
	while (1)
	{
	 uint8_t keyn = 0;
         keyn = get_adc_key();
         if(keyn != 0)
         {
               printf("key: %d\n", keyn);
               keynun++;
               printf("key: %d\n", keynun);
         }
      
		vTaskDelay(300 / portTICK_PERIOD_MS);
	}
}


