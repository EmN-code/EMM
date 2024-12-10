#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "dht.h"

#include "thread.h"
#include "xtimer.h"

#include <stdio.h>

// Constants
#define DHT_PIN GPIO_PIN(0,5)  // GPIO pin for DHT11
#define LED_PIN GPIO_PIN(0,2)  // GPIO pin for LED
#define TEMP_THRESHOLD 30      // Temperature threshold

void *read_dht(void *arg) {
    (void)arg;
    int16_t temp, hum;
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        if (dht_read(DHT11, DHT_PIN, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
            xtimer_sleep(5);
        } else {
            printf("Failed to read from DHT11 sensor\n");
            xtimer_sleep(1);
        }
    }
    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, read_dht, NULL, "dht_thread");
    return 0;
}