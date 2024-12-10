#include <thread.h>
#include <timex.h>
#include <stdio.h>
#include <stdbool.h>
#include <periph/gpio.h>
#include <msg.h>
#include <dht.h>

#define DHT11_PIN GPIO_PIN(0, 2)
#define INTERVAL_S (5U)

int main(void) {
    // Initialize the DHT sensor
    if (dht_init(DHT11_PIN) != DHT_OK) {
        printf("DHT initialization failed\n");
        return -1;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(DHT11_PIN, &temp, &hum) == DHT_OK) {
            // Print temperature and humidity
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        }
        else {
            printf("Failed to read from DHT sensor\n");
        }

        // Wait for 5 seconds
        xtimer_sleep(INTERVAL_S);
    }

    return 0;
}