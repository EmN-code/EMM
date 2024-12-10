
#include "periph/gpio.h"
#include "periph/timer.h"
#include "dht.h"
#include "xtimer.h"
#include <stdio.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define TIMER_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static dht_t dht_dev;

void read_dht(void) {
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        puts("Failed to read from DHT sensor");
    }
}

int main(void) {
    // Initialize the DHT11 sensor
    if (dht_init(&dht_dev, DHT_TYPE_DHT11, DHT_PIN) != DHT_OK) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    // Set up a timer to read the DHT11 every 5 seconds
    xtimer_t timer;
    timer.callback = (xtimer_callback_t)read_dht;
    timer.arg = NULL;
    xtimer_set(&timer, TIMER_INTERVAL);

    // Keep the main thread alive
    while (1) {
        xtimer_usleep(1000000); // Sleep for 1 second
    }

    return 0;
}
