#include <xtimer.h>
#include <stdio.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT11_PIN GPIO_PIN(0, 2)
#define INTERVAL 5000000    // 5 seconds in microseconds

int main(void) {
    // Initialize DHT sensor at the specified GPIO pin
    dht_params_t dht_params = {
        .pin = DHT11_PIN,
        .type = DHT11
    };
    dht_t dev;

    if (dht_init(&dev, &dht_params) != 0) {
        printf("Error: Could not initialize DHT11 sensor\n");
        return -1;
    }

    int16_t temperature, humidity;

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                    temperature / 10, temperature % 10, 
                    humidity / 10, humidity % 10);
        } else {
            printf("Error: Failed to read data from DHT11 sensor\n");
        }

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL);
    }
    return 0;
}