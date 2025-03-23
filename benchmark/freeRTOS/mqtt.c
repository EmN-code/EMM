#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "mqtt_client.h"


// WiFi credentials
#define WIFI_SSID "EmNets-301"
#define WIFI_PASS "eagle402"
int connected = 0;

// MQTT
#define MQTT_URI "mqtt://10.214.131.229"
#define MQTT_PORT 38883
#define TOPIC "topic/test"
#define PAYLOAD "Hello MQTT!"
#define QOS 1


// tag for log
static const char *TAG = "MQTT_CLIENT";

// Event handler for WiFi
static TaskHandle_t wifi_task_handle = NULL;

// Event handler for WiFi
static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START){
        esp_wifi_connect();
        ESP_LOGI(TAG, "WiFi Connecting...");
    }else if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED){
        ESP_LOGI(TAG, "WiFi Connected");
    }else if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED){
        ESP_LOGI(TAG, "WiFi Disconnected. Trying to reconnect...");
        esp_wifi_connect();
    }else if(event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP){
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        connected = 1;
    }
}

//init wifi
void wifi_connect_task(void *pvParameters){
    // init nvs
    ESP_ERROR_CHECK(nvs_flash_init());
    // init tcp/ip adapter
    ESP_ERROR_CHECK(esp_netif_init());
    // create default event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    // init wifi config
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    // register wifi event handler
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, NULL));
    // start to connect
    ESP_ERROR_CHECK(esp_wifi_start());
    
    ESP_LOGI(TAG, "WiFi init finished.");
    
    // keep running
    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data){
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32 "", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        esp_mqtt_client_publish(client, TOPIC, PAYLOAD, 0, QOS, 0);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        ESP_LOGI(TAG, "TOPIC=%.*s\r\n", event->topic_len, event->topic);
        ESP_LOGI(TAG, "DATA=%.*s\r\n", event->data_len, event->data);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;

    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

void mqtt_task(void *pvParameters){
    // wait for wifi connection
    while(connected == 0){
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = MQTT_URI,
        .broker.address.port = MQTT_PORT
    };
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    xTaskCreate(
        wifi_connect_task,   // function to implement the task
        "wifi_connect_task", // name of the task
        4096,                // stack size in words
        NULL,                // task input parameter
        configMAX_PRIORITIES-1, // priority of the task
        &wifi_task_handle   // task handle
    );
    xTaskCreate(
        mqtt_task,   // function to implement the task
        "mqtt_task", // name of the task
        4096,        // stack size in words
        NULL,        // task input parameter
        configMAX_PRIORITIES-2, // priority of the task
        NULL         // task handle
    );
}