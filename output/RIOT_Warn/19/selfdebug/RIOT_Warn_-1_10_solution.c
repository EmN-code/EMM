#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT11_PIN GPIO_PIN(0, 4) // Replace with your actual GPIO pin number for DHT11
#define LED_PIN GPIO_PIN(0, 5)  // Replace with your actual GPIO pin number for LED
#define TEMP_THRESHOLD 30       // Temperature threshold for warning in Celsius
#define SLEEP_INTERVAL (5U * US_PER_SEC) // 5 seconds interval

static dht_t dev;

void *dht_read_thread(void *arg) {
    (void)arg;
    int16_t temp, hum;
    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("Error reading DHT11\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp > TEMP_THRESHOLD * 10) {  // temp is read in tenths of a degree
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    printf("Initialize DHT11 on pin %d\n", DHT11_PIN);

    if (dht_init(&dev, DHT11_PIN, DHT_NONBLOCK) == DHT_OK) {
        printf("DHT11 initialized successfully\n");
    } else {
        printf("DHT11 initialization failed\n");
        return 1;
    }

    gpio_init(LED_PIN, GPIO_OUT);

    static char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, dht_read_thread, NULL, "dht_read");

    return 0;
}