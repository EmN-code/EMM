
#include "periph/gpio.h"
#include "periph/dht.h"
#include "xtimer.h"
#include "timex.h"

#define DHT_PIN             GPIO_PIN(0, 2)  // GPIO 2 on ESP32
#define DHT_TYPE            DHT11           // Specify the DHT type
#define SLEEP_INTERVAL      (5 * 1000 * 1000u)  // 5 seconds in microseconds

int main(void) {
    dht_t dev;
    int16_t temp, hum;

    // Initialize the DHT11 sensor
    if (dht_init(&dev, DHT_PIN, DHT_TYPE) != DHT_OK) {
        printf("DHT11 initialization failed\n");
        return 1;
    }

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read data from DHT11\n");
        }

        // Sleep for 5 seconds
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
