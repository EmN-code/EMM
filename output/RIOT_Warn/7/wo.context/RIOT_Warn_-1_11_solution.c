#include <thread.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <xtimer.h>
#include "dht.h"

#define DHT_PIN         GPIO_PIN(PORT_A, 0) // Assuming GPIO port A
#define LED_PIN         GPIO_PIN(PORT_B, 1) // Assuming GPIO port B
#define TEMP_THRESHOLD  25
#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)

static char thread_stack[THREAD_STACKSIZE];

void *sensor_thread(void *arg) {
    dht_t dev;
    if (dht_init(&dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        printf("Error initializing DHT sensor\n");
        return NULL;
    }

    int16_t temp, hum;
    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Error reading DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C\n", temp / 10, temp % 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Warning: Temperature exceeds threshold!\n");
            } else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  0, sensor_thread, NULL, "sensor_thread");
    return 0;
}