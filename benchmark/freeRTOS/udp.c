#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_netif.h"

#include "lwip/sockets.h"


// WiFi credentials
#define WIFI_SSID "EmNets-301"
#define WIFI_PASS "eagle402"
int connected = 0;

//udp
#define HOST_IP "10.214.149.209"
#define HOST_PORT 1234
#define PAYLOAD "Hello UDP from ESP32!"

// tag for log
static const char *TAG = "UDP_CLIENT";

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

static void udp_client_task(void *pvParameters){
    // wait for wifi connection
    while(connected == 0){
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    
    char rx_buffer[128];
    int addr_family = 0;
    int ip_protocol = 0;

    while(1){
        struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = inet_addr(HOST_IP);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(1234);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;

        int sock = socket(addr_family, SOCK_DGRAM, ip_protocol); //SOCK_DGRAM for UDP
        if(sock < 0){
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            break;
        }

        // set timeout
        struct timeval timeout;
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

        ESP_LOGI(TAG, "Socket created, sending to %s:%d", HOST_IP, HOST_PORT);

        while(1){
            int err = sendto(sock, PAYLOAD, strlen(PAYLOAD), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
            if(err<0){
                ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                break;
            }
            ESP_LOGI(TAG, "Message sent");
            
            struct sockaddr_in source_addr;
            socklen_t socklen = sizeof(source_addr);
            int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);
            if(len<0){
                ESP_LOGE(TAG, "Error occurred during receiving: errno %d", errno);
                break;
            }else{
                rx_buffer[len] = 0;
                ESP_LOGI(TAG, "Received %d bytes from %s:", len, HOST_IP);
                ESP_LOGI(TAG, "%s", rx_buffer);
                if(strcmp(rx_buffer, "OK") == 0){
                    ESP_LOGI(TAG, "Received OK from server. Task finished.");
                    break;
                }
            }
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        if(sock!=-1){
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    vTaskDelete(NULL);
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
        udp_client_task,   // function to implement the task
        "udp_client_task", // name of the task
        4096,                // stack size in words
        NULL,                // task input parameter
        configMAX_PRIORITIES-2, // priority of the task
        NULL   // task handle
    );
}