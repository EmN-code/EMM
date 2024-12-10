#include <xtimer.h>
#include <stdio.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT11_PIN GPIO_PIN(PORT_A, 2)
#define DHT_TYPE DHT11
#define INTERVAL (5U) * US_PER_SEC

int main(void) {
    dht_params_t dht_params;
    dht_t dev;
    int16_t temp, hum;
    int result;

    // Pin initialization
    gpio_init(DHT11_PIN, GPIO_IN);

    // Initialize DHT device with proper parameters
    while((result = dht_init(&dev, &dht_params, DHT11_PIN, DHT_TYPE)) != DHT_OK) {
        printf("Failed to initialize DHT11. Retrying...\n");
        xtimer_sleep(1);
    }
    printf("DHT11 initialized successfully.\n");

    while (1) {
        xtimer_sleep(INTERVAL);

        // Read temperature and humidity
        result = dht_read(&dev, &temp, &hum);
        if (result == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        }
        else {
            printf("Failed to read from DHT11 sensor. Code: %d\n", result);
        }
    }

    return 0;
}