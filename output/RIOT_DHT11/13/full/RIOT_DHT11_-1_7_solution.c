#include "paho_mqtt.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "thread_flags.h"
#include "thread.h"

#define DHT_PIN 2
#define THREAD_STACKSIZE 1024
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MQTT_BROKER_IP "192.168.1.100"
#define MQTT_BROKER_PORT 1883

void *dht_read_thread(void *arg) {
    dht_t dev;
    int16_t temp, hum;
    int ret;

    /* Initialize DHT sensor */
    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != 0) {
        // handle init error
        return NULL;
    }

    while (1) {
        ret = dht_read(&dev, &temp, &hum);
        if (ret != 0) {
            // handle read error
        } else {
            // process valid data
        }

        usleep(5000000); /* Sleep for 5 seconds */
    }

    return NULL;
}

int main(void) {
    /* Initialize NVS */
    if (nvs_flash_init() != ESP_OK) {
        // handle NVS initialization error
        return 1;
    }

    /* Create DHT reading thread */
    char stack[THREAD_STACKSIZE];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY, THREAD_CREATE_STACKTEST, dht_read_thread, NULL, "dht_reader");

    /* Network Attempt Connection Logic */
    Network network;
    int rc;
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            // Connection failed, retry
            usleep(1000000); /* Sleep for 1 second */
            continue;
        } else {
            // Connection successful
            break;
        }
    }

    // Further MQTT connection and data publishing logic

    return 0;
}