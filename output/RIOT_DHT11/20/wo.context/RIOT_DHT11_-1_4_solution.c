#include "xtimer.h"
#include "thread_flags.h"
#include "dht.h"
#include "periph/gpio.h"

#define DHT11_PIN GPIO_PIN(0, 2)

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    
    // Initialize the DHT11 sensor
    if (dht_init(&dev, DHT11_PIN) != DHT_OK) {
        puts("Failed to initialize DHT device");
        return 1;
    }

    while (1) {
        // Read the temperature and humidity from DHT11
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            puts("Failed to read data from sensor");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}