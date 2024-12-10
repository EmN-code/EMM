
#include <stdio.h>
#include <xtimer.h>
#include <dht.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO 2 on ESP32
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static dht_t dht11;

int main(void)
{
    // Initialize the DHT11 sensor
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT11,
        .in_mode = DHT_PARAM_PULL,
    };

    if (dht_init(&dht11, &dht_params) == DHT_OK) {
        printf("DHT11 sensor initialized successfully\n");
    } else {
        printf("Failed to initialize DHT11 sensor\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;

        // Read temperature and humidity
        if (dht_read(&dht11, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read data from DHT11 sensor\n");
        }

        // Wait for 5 seconds
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
