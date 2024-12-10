#include <xtimer.h>  // For using timers
#include <periph/gpio.h>  // For GPIO operations
#include "dht.h"  // DHT library header

#define DHT_PIN GPIO_PIN(0, 2)  // DHT11 connected to GPIO2
#define DHT_TYPE DHT11  // We're using a DHT11 sensor
#define INTERVAL (5U * US_PER_SEC)  // 5 seconds interval

int main(void) {
    // Initialize the DHT sensor
    if (dht_init(DHT_PIN) != 0) {
        printf("Error: unable to initialize DHT sensor\n");
        return 1;
    }

    int16_t temp, hum;  // Variables to store temperature and humidity values

    while (1) {
        // Read values from the DHT sensor
        if (dht_read(DHT_TYPE, DHT_PIN, &temp, &hum) != 0) {
            printf("Error: unable to read from DHT sensor\n");
        } else {
            // Print the temperature and humidity
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        }

        // Wait for the next interval
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}