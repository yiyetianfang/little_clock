#include "display_app.h"
#include "get_weather.h"
#include "bg_pic.h"

static void lv_tick_task(void *arg);

static lv_style_t style_obj;
lv_obj_t * scr;
lv_obj_t * label1;
lv_obj_t * label2;
lv_obj_t * label3;
lv_obj_t * label4;
lv_obj_t * label5;
char text[16] = "100ash.nret";
		

static void weather_ui_init()
{
    //static lv_style_t style_obj;
    /* Get the current screen  */
    scr = lv_disp_get_scr_act(NULL);   
    lv_obj_align(scr, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_border_width(scr, 2, 0);
    /*Create a Label on the currently active screen*/
     label1 =  lv_label_create(scr);
     label2 =  lv_label_create(scr);
     label3 =  lv_label_create(scr);
     label4 =  lv_label_create(scr);
     label5 =  lv_label_create(scr);
    LV_FONT_DECLARE(font_source_han_sans_bold_12);

    lv_style_init(&style_obj);
    lv_style_set_bg_color(&style_obj, lv_color_hex(0xc43e1c));      // 设置背景色
    lv_style_set_text_color(&style_obj,lv_color_hex(0xfffe1c));
    lv_obj_set_style_text_color(label1,lv_color_hex(0xff0000),0);
    lv_obj_set_style_text_color(label2,lv_color_hex(0xf7b37b),0);
    lv_obj_set_style_text_color(label3,lv_color_hex(0xf7b37b),0);
    lv_obj_set_style_text_color(label4,lv_color_hex(0xf7b37b),0);
    lv_obj_set_style_text_color(label5,lv_color_hex(0xf7b37b),0);
    lv_obj_set_style_text_font(label1,&font_source_han_sans_bold_12,0);
    lv_obj_set_style_text_font(label2,&font_source_han_sans_bold_12,0);
    lv_obj_set_style_text_font(label3,&font_source_han_sans_bold_12,0);
    lv_obj_set_style_text_font(label4,&font_source_han_sans_bold_12,0);
    lv_obj_set_style_text_font(label5,&font_source_han_sans_bold_12,0);
    lv_obj_set_size(label1, 250, 20);
    lv_obj_align(label1, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_size(label2, 250, 20);
    lv_obj_align_to(label2, label1, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    lv_obj_set_size(label3, 250, 20);
    lv_obj_align_to(label3, label2, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    lv_obj_set_size(label4, 250, 20);
    lv_obj_align_to(label4, label3, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    lv_obj_set_size(label5, 250, 20);
    lv_obj_align_to(label5, label4, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
}

static void create_demo_application(void)
{
#if 1
    lv_obj_set_style_text_color(label1,lv_color_hex(0x66FFFF),0);
    lv_label_set_text(label1,datestr[0]);
   // lv_label_set_text_fmt(label1,"#FF0000 %s",datestr[0]);
     lv_label_set_text(label2,tempstr[0]);
 
    lv_label_set_text(label3,humiditystr[0]);

    lv_label_set_text(label4,Rainfallstr[0]);

     lv_label_set_text_fmt(label5, "%s: %d", "Value", keynun);
   
#endif
    // lv_example_get_started_1();
      //lv_example_btnmatrix_2();
      //  lv_example_img_1();
    
}



/* Creates a semaphore to handle concurrent call to lvgl stuff
 * If you wish to call *any* lvgl function from other threads/tasks
 * you should lock on the very same semaphore! */
SemaphoreHandle_t xGuiSemaphore;
 
 void guiTask(void *pvParameter)
{
    (void) pvParameter;
    xGuiSemaphore = xSemaphoreCreateMutex();
 
    lv_init();
 
    /* Initialize SPI or I2C bus used by the drivers */
    lvgl_driver_init();
 
    lv_color_t *buf1 = heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf1 != NULL);
    /* Use double buffered when not working with monochrome displays */
    lv_color_t* buf2 = heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf2 != NULL);
    static lv_disp_draw_buf_t disp_buf;
    uint32_t size_in_px = DISP_BUF_SIZE;
    /* Initialize the working buffer depending on the selected display. */
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, size_in_px);
 
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = LV_HOR_RES_MAX;
    disp_drv.ver_res = LV_VER_RES_MAX;
    disp_drv.flush_cb = disp_driver_flush;
    disp_drv.draw_buf = &disp_buf;
    lv_disp_drv_register(&disp_drv);
 
    /* Create and start a periodic timer interrupt to call lv_tick_inc */
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .name = "periodic_gui"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));
 
    lv_port_indev_init();
    /* Create the demo application */
   // create_demo_application();
   //lv_img_dsc_bg_dis();
    lv_img_dsc_bg_dis();
    weather_ui_init();
    while (1)
    {
        /* Delay 1 tick (assumes FreeRTOS tick is 10ms */
        vTaskDelay(pdMS_TO_TICKS(10));
        lv_task_handler();
        //lv_label_set_text_fmt(label5, "%s: %d", "Value", keynun);
        if(get_flag ==true) 
        {
            get_flag = false; 
             create_demo_application();
        }
        /* Try to take the semaphore, call lvgl related function on success */
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY)) {
            lv_task_handler();
            xSemaphoreGive(xGuiSemaphore);
       }
    }
}
 

 
static void lv_tick_task(void *arg) {
    (void) arg;
 
    lv_tick_inc(LV_TICK_PERIOD_MS);
}