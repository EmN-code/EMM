
#include "periph/gpio.h"
#include "ztimer.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define INTERVAL (5 * 1000U)  // 5 seconds in milliseconds

int main(void) {
    int16_t temperature, humidity;

    // Initialize the DHT11 sensor
    dht_t dev;
    if (dht_init(&dev, DHT_PIN) == DHT_OK) {
        while (1) {
            // Read temperature and humidity
            if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
                // Print the values
                printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                       temperature / 10, temperature % 10,
                       humidity / 10, humidity % 10);
            } else {
                puts("Failed to read from DHT sensor");
            }

            // Wait for 5 seconds
            ztimer_sleep(ZTIMER_MSEC, INTERVAL);
        }
    } else {
        puts("Failed to initialize DHT sensor");
    }

    return 0;
}
