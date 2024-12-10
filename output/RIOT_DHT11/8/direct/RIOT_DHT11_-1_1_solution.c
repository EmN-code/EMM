#include <ztimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(PORT_A, 2)
#define SLEEP_INTERVAL (5U * 1000U * 1000U)  // 5 seconds in microseconds

dht_t dev;

int main(void) {
    int16_t temp, hum;
    dht_params_t params;

    // Initialize the DHT11 sensor
    params.pin = DHT_PIN;
    params.type = DHT11;
    dht_init(&dev, &params);

    while (1) {
        // Read data from DHT11 sensor
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }
        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, SLEEP_INTERVAL);
    }
    return 0;
}