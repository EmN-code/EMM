#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>
#include "xtimer.h"
#include "ztimer.h"
#include "thread.h"
#include "dht.h"
#include "msg.h"
#include "netinet/in.h"
#include "architecture.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

#define DHT11_PIN 2
#define SLEEP_INTERVAL (5U * 1000U * 1000U) // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_MAIN];

void *dht_thread(void *arg) {
    (void)arg;
    uint16_t temperature, humidity;
    while (1) {
        if (dht_read(DHT11, DHT11_PIN, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            printf("Error reading DHT11\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    mqtt_client_t mqtt_client;
    msg_init_queue(mqtt_client.queue, sizeof(mqtt_client.queue)/sizeof(*mqtt_client.queue));

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, dht_thread, NULL, "dht_reader");

    // Initialize MQTT with appropriate network settings (user-defined)
    // Setup MQTT client and connect

    return 0;
}