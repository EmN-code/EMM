#include "paho_mqtt.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "event/thread.h"
#include "MQTTClient.h"
#include "dht.h"
#include "thread.h"
#include "dht_params.h"
#include "mma8x5x_params.h"

#define TEMP_THRESHOLD 30
#define DHT_PIN GPIO_NUM_4
#define LED_PIN GPIO_NUM_2

void *dht_thread(void *arg) {
    mqtt_client_t mqttClient;
    network_t network;
    mqtt_client_connect_info_t connect_info = MQTTClientConnectInfo_initializer;
    int rc;
    
    while (1) {
        /* Read temperature and humidity values from the DHT11 sensor */
        int16_t temperature, humidity;
        if (dht_read(DHT_PIN, &temperature, &humidity) != 0) {
            continue; // If reading fails, retry
        }

        /* Check if the temperature exceeds the threshold */
        if (temperature > TEMP_THRESHOLD) {
            gpio_set(LED_PIN); // Turn on LED warning
        } else {
            gpio_clear(LED_PIN); // Ensure LED is off otherwise
        }

        /* Attempt MQTT connection and publish */
        if ((rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT)) != 0) {
            printf("Network connection failed.");
            xtimer_sleep(1);
            continue;
        }

        /* Initialize MQTT client and connect */
        connack_data_t connack_data;
        MQTTClientInit(&mqttClient, &network, 1000, NULL, 0, NULL, 0);
        connect_info.keepalive_interval = 10;
        connect_info.cleansession = 1;

        while ((rc = MQTTConnect(&mqttClient, &connect_info)) != 0) {
            printf("MQTT connection failed: %d", rc);
            xtimer_sleep(1);
        }

        /* Publish temperature data */
        char payload[50];
        sprintf(payload, "Temperature: %d", temperature);
        rc = MQTTPublish(&client, "sensor/temperature", payload);
        
        if (rc != MQTT_SUCCESS) {
            printf("Failed to publish message: %d", rc);
        }

        xtimer_sleep(5); // Delay for 5 seconds before next read
    }
    return NULL;
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, dht_thread, NULL, "dht_reader");

    return 0;
}