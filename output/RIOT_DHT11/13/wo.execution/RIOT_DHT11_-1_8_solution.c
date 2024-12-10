#include <ztimer.h>
#include <unistd.h>
#include <stdlib.h>
#include <thread.h>
#include <timex.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <netinet/in.h>
#include "net/sock/udp.h"
#include "msg.h"
#include "mbox.h"
#include "MQTTClient.h"
#include "evtimer_mbox.h"
#include "periph/gpio.h"
#include "dht.h"

#define GPIO_DHT 2
#define DELAY_MS (5U * 1000U)

static char stack[THREAD_STACKSIZE_MAIN];

void *sensor_thread(void *arg) {
    (void)arg;

    int16_t temperature, humidity;

    // Set GPIO as input for the DHT sensor
    gpio_init(GPIO_DHT, GPIO_IN);

    while (1) {
        // Read data from the sensor
        if (dht_read(&dht_dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temperature / 10, temperature % 10, 
                   humidity / 10, humidity % 10);
        }
        else {
            printf("Failed to read from sensor\n");
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, DELAY_MS);
    }
    return NULL;
}

int main(void) {
    printf("Starting sensor application\n");
    
    // Create a separate thread to handle sensor readings
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  sensor_thread, NULL, "sensor_thread");
    
    // The main thread can perform other network tasks, like setting up the MQTT connection
    // or handling other peripheral tasks such as display, if needed
    while (1) {
        // Main thread task can be implemented here
        // Placeholder for potential MQTT task or other functionalities
        ztimer_sleep(ZTIMER_MSEC, DELAY_MS);
    }

    return 0;
}