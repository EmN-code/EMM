#include "thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "dht_params.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DHT_PIN GPIO_PIN(PORT_A, 0) // Define the GPIO pin for DHT11
#define LED_PIN GPIO_PIN(PORT_B, 1) // Define the GPIO pin for LED
#define TEMP_THRESHOLD 25 // Temperature threshold in Celsius

static char stack[THREAD_STACKSIZE_MAIN];

void *dht_reader(void *arg) {
    (void)arg;
    int16_t temp, hum;
    dht_t dev;
    dev.pin = DHT_PIN;
    dev.type = DHT11;

    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp > TEMP_THRESHOLD * 10) { // Convert threshold to deci-Celsius
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    printf("Starting DHT11 temperature monitoring\n");
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, dht_reader, NULL, "dht_reader");
    while (1) {
        thread_sleep(); // Let the thread take over
    }
    return 0;
}