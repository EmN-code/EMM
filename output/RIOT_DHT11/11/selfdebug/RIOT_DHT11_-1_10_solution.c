#include <xtimer.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define SLEEP_TIME (5U) // Sleep time in seconds

int main(void) {
    // Initialize the DHT sensor
    dht_t dev;
    dev.params.pin = DHT_PIN;
    dht_init(&dev);

    while (1) {
        int16_t temp, hum;
        // Read values from the DHT11 sensor
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        // Sleep for 5 seconds
        xtimer_sleep(SLEEP_TIME);
    }

    return 0;
}