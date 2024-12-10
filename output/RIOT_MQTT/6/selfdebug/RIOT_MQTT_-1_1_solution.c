#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqtt_client.h>
#include <esp_wifi.h>
#include <netdb.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL 5
#define TOPIC "esp32/status"

static const char* broker_uri = "mqtt://47.102.103.1";

static void log_error_if_nonzero(const char *message, int error_code) {
    if (error_code != 0) {
        printf("%s failed: %d\n", message, error_code);
    }
}

void publish_task(void *pvParameters) {
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = broker_uri,
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_start(client);

    char *status_message = "work";
    while (1) {
        esp_mqtt_client_publish(client, TOPIC, status_message, 0, 0, 0);
        status_message = (strcmp(status_message, "work") == 0) ? "not work" : "work";
        vTaskDelay(PUBLISH_INTERVAL * 1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void) {
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_TCP", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

    // Initialize WiFi
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();

    // Connecting to the WiFi
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "your_ssid",
            .password = "your_password"
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_connect());

    // Start the MQTT publishing task
    xTaskCreate(&publish_task, "publish_task", 4096, NULL, 5, NULL);
}