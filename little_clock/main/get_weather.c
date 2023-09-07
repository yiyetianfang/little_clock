#include "get_weather.h"

static const char *HTTP_TAG = "httpTask";
#define MAX_HTTP_OUTPUT_BUFFER 1300
#define HOST "api.seniverse.com"
#define UserKey "Sed_lnwUBFUn2yVth"
#define Location "shenzhen"
#define Language "zh-Hans"
#define Strat "0"
#define Days "5"

char datestr[3][20] ;
char tempstr[3][20];
char Rainfallstr[3][20];
char humiditystr[3][20];
uint8_t get_flag = false;

// static void freeBuffer(char* buf, int bufSize){
// 	for( int i = 0; i < bufSize; i ++ )
// 		buf[i] = NULL;	
// }

static void cJSON_parse_task(char* text){
	//char oledText[16];
	char *date,*Temp_High = NULL,*Temp_Low,*humidity,*Rainfall;
	cJSON *root,*arrayItem,*subArray;
	cJSON *arr_item,*sub_array_item;
	cJSON *JsonDate,*JsonTemp_High,*JsonTemp_Low,*JsonHumidity,*JsonRainfall;
	root = cJSON_Parse(text);
	if(root!=NULL)
	{
		arrayItem = cJSON_GetObjectItem(root,"results");
		//ESP_LOGE(HTTP_TAG, "Data IS json");
		int arr_size = cJSON_GetArraySize(arrayItem);
		ESP_LOGI(HTTP_TAG, "root_arr_size: %d \n", arr_size);
		arr_item = arrayItem->child;
		for(int i = 0; i < arr_size; i ++){
			subArray = cJSON_GetObjectItem(arr_item, "daily");
			int sub_array_size = cJSON_GetArraySize(subArray);
			sub_array_item = subArray->child;
			ESP_LOGI(HTTP_TAG, "sub_arr_size: %d \n", sub_array_size);
			for(int j = 0; j < sub_array_size; j ++){
				if(sub_array_item->type == cJSON_Object){
					JsonDate =  cJSON_GetObjectItem(sub_array_item, "date");
					if(cJSON_IsString(JsonDate)){
						date = JsonDate->valuestring;
						sprintf(datestr[j], "date: %s", date);
                        printf("%s\n",datestr[j]);
					}
					
					JsonTemp_High =  cJSON_GetObjectItem(sub_array_item, "high");
					if(cJSON_IsString(JsonTemp_High))
						Temp_High = JsonTemp_High->valuestring;
					
					JsonTemp_Low =  cJSON_GetObjectItem(sub_array_item, "low");
					if(cJSON_IsString(JsonTemp_Low)){
						Temp_Low=JsonTemp_Low->valuestring;
						sprintf(tempstr[j], "Temp: %s to %s", Temp_High, Temp_Low);
                         printf("%s\n", tempstr[j]);
					}
					
					JsonHumidity =  cJSON_GetObjectItem(sub_array_item, "humidity");
					if(cJSON_IsString(JsonHumidity)){
						humidity=JsonHumidity->valuestring;
						sprintf(humiditystr[j], "Humi: %s", humidity);
                        printf("%s\n", humiditystr[j]);
					}

					JsonRainfall =  cJSON_GetObjectItem(sub_array_item, "rainfall");
					if(cJSON_IsString(JsonRainfall)){
						Rainfall=JsonRainfall->valuestring;
						sprintf(Rainfallstr[j], "Rain: %s", Rainfall);
                        printf("%s\n", Rainfallstr[j]);
					}
					vTaskDelay(2000 / portTICK_PERIOD_MS);
				}
				sub_array_item = sub_array_item->next;
			}
			arr_item = arr_item -> next;
		}
		ESP_LOGI(HTTP_TAG, "Finish");
		get_flag = true;
	}
	cJSON_Delete(root);
}

/** HTTP functions **/
static void http_client_task(void *pvParameters){
	char output_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};   // Buffer to store response of http request
    int content_length = 0;
	static const char *URL = "http://"HOST"/v3/weather/daily.json?"	\
							 "key="UserKey"&location="Location		\
							 "&language="Language					\
							 "&unit=c&start="Strat"&days="Days;
    esp_http_client_config_t config = {
        .url = URL,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    // GET Request
    esp_http_client_set_method(client, HTTP_METHOD_GET);
    esp_err_t err = esp_http_client_open(client, 0);
    if (err != ESP_OK) {
        ESP_LOGE(HTTP_TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
    } else {
        content_length = esp_http_client_fetch_headers(client);
        if (content_length < 0) {
            ESP_LOGE(HTTP_TAG, "HTTP client fetch headers failed");
        } else {
            int data_read = esp_http_client_read_response(client, output_buffer, MAX_HTTP_OUTPUT_BUFFER);
            if (data_read >= 0) {
                ESP_LOGI(HTTP_TAG, "HTTP GET Status = %d, content_length = %d",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
                printf("data:%s", output_buffer);
				cJSON_parse_task(output_buffer);
            } else {
                ESP_LOGE(HTTP_TAG, "Failed to read response");
            }
        }
    }
   esp_http_client_close(client);
   vTaskDelete(NULL);
}

void get_weather_app(void)
{
   xTaskCreate(http_client_task, "http_client", 5120, NULL, 3, NULL);
}






