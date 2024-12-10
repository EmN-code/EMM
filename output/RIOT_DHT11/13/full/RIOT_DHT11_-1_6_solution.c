#include <stdio.h>
#include <stdint.h>
#include <xtimer.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "thread.h"
#include "event/thread.h"
#include "dht.h"
#include "dht_params.h"
#include "thread_flags.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"

#define GPIO_DHT11 2
#define SLEEP_INTERVAL   (5U * US_PER_SEC) // 5 seconds

void *dht11_thread(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temperature = 0; // To store temperature value
    int16_t humidity = 0;    // To store humidity value

    // Initialize DHT11 sensor
    if (dht_init(&dev, GPIO_DHT11) != 0) {
        printf("Error initializing DHT11\n");
        return NULL;
    }

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("Read error from DHT11\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        }
        xtimer_sleep(5); // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, dht11_thread, NULL, "dht11_thread");
    return 0;
}