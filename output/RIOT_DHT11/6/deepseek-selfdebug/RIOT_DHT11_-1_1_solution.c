
#include "xtimer.h"
#include "dht.h"
#include "periph/gpio.h"
#include "fmt.h"

#define DHT_PIN             GPIO_PIN(0, 2)  // GPIO 2 on ESP32
#define INTERVAL_SECONDS    5

static dht_t dht11;

int main(void)
{
    // Initialize the DHT11 sensor
    dht_params_t dht_params = {
        .pin = DHT_PIN,
        .type = DHT11,
        .in_mode = DHT_PARAM_PULL,
    };

    if (dht_init(&dht11, &dht_params) != DHT_OK) {
        puts("DHT11 initialization failed");
        return 1;
    }

    while (1) {
        int16_t temp, hum;

        // Read temperature and humidity from the DHT11 sensor
        if (dht_read(&dht11, &temp, &hum) != DHT_OK) {
            puts("Failed to read from DHT11 sensor");
        } else {
            // Print the temperature and humidity values
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        }

        // Wait for 5 seconds before the next reading
        xtimer_sleep(INTERVAL_SECONDS);
    }

    return 0;
}
