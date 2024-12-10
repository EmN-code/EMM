#include <xtimer.h>  // For timer operations
#include "dht.h"  // For DHT sensor operations
#include "periph/gpio.h"  // For GPIO operations

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO pin 2 for ESP32
#define DHT_TYPE DHT11  // DHT11 sensor type

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, &DHT_PIN, DHT_TYPE) != 0) {
        // Initialize DHT sensor
        printf("Error: Unable to initialize the DHT sensor\n");
        return -1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            // Successfully read data from DHT sensor
            printf("Temperature: %d.%dC, Humidity: %d.%d\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        xtimer_sleep(5);  // Wait for 5 seconds before the next reading
    }

    return 0;
}