#include <xtimer.h>  // Required for delays and timers
#include <thread.h>  // Required for threading support
#include <dht.h>  // Required for interfacing with the DHT sensor

#define DELAY_TIME 5000000 // 5 seconds in microseconds
#define DHT_PIN 2  // GPIO pin where the DHT11 is connected

int main(void) {
    dht_t dev;
    int16_t temperature, humidity;
    dht_params_t params = {
        .type = DHT_TYPE_DHT11,
        .pin = DHT_PIN
    };

    if (dht_init(&dev, &params) != DHT_OK) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    while (1) {
        // Read temperature and humidity from DHT11
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            puts("Failed to read data from DHT sensor");
        }

        // Sleep for 5 seconds
        xtimer_usleep(DELAY_TIME);
    }
    return 0;
}