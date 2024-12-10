#include <dht.h>
#include "xtimer.h"
#include "timex.h"
#include "stdio.h"
#include "thread.h"
#include "periph/gpio.h"

// DHT device and GPIO configuration
#define DHT_PIN GPIO_PIN(0, 2)
#define DHT_TYPE DHT11

void read_dht(void) {
    dht_t dev;
    dht_params_t params;
    int16_t temp, hum;
    int res;

    // Initialize DHT parameters
    params.pin = DHT_PIN;
    params.type = DHT_TYPE;
    
    // Initialize the DHT device
    if (dht_init(&dev, &params) != DHT_OK) {
        printf("Failed to initialize DHT\n");
        return;
    }

    while (1) {
        // Read temperature and humidity
        res = dht_read(&dev, &temp, &hum);
        if (res == DHT_OK) {
            printf("Temperature: %d.%dC, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read data from DHT\n");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }
}

int main(void) {
    // Configure the GPIO pin for the DHT sensor
    gpio_init(DHT_PIN, GPIO_IN);
    
    // Start the DHT reading thread
    thread_create(NULL, 1024, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, read_dht, NULL, "dht_reader");

    // Keep the main alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}