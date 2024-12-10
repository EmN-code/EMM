#include <stdio.h>
#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 5)    // Adjust this to the actual pin connected to DHT11
#define LED_PIN GPIO_PIN(PORT_A, 6)    // Pin connected to LED
#define THRESHOLD_TEMP 25

// Define the DHT parameters
static dht_params_t my_dht_params = {
    .pin = DHT_PIN,
    .type = DHT11
};

dht_t dev;

void *read_dht(void *arg) {
    (void)arg;
    int16_t temp, hum;
    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 > THRESHOLD_TEMP) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read sensor\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the DHT sensor
    if (dht_init(&dev, &my_dht_params) != DHT_OK) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }

    // Create a thread to read the DHT sensor
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_dht, NULL, "read_dht");

    return 0;
}