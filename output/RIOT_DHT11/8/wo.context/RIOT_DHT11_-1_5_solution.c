#include "xtimer.h"
#include "timex.h"
#include "stdio.h"
#include "periph/gpio.h"
#include "dht.h"

#define INTERVAL (5U * US_PER_SEC) // 5 seconds
#define DHT_PIN GPIO_PIN(0, 2)    // DHT11 is connected to GPIO 2

int main(void) {
    // Initialize DHT sensor
    if (dht_init(DHT_PIN) != DHT_OK) {
        printf("Error: Unable to initialize DHT sensor!\n");
        return -1;
    }

    int16_t temperature, humidity;

    while (1) {
        // Read the DHT sensor
        if (dht_read(DHT_PIN, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temperature / 10, temperature % 10, 
                   humidity / 10, humidity % 10);
        } else {
            printf("Error reading DHT sensor\n");
        }

        // Sleep for the defined interval
        xtimer_sleep(INTERVAL);
    }

    return 0;
}