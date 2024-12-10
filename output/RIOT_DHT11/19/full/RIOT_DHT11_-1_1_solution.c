#include "thread_config.h"
#include "periph/gpio.h"
#include "dht.h"
#include "xtimer.h"

int main(void) {
    // Define DHT11 sensor parameters
    dht_t dev;
    int16_t temp, hum;
    dht_params_t params;

    // Initialize DHT11 with GPIO 2
    params.pin = GPIO_PIN(0, 2);
    params.type = DHT_TYPE_DHT11;
    if (dht_init(&dev, &params) != 0) {
        printf("DHT11 initialization failed\n");
        return 1;
    }

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Failed to read DHT11 sensor\n");
        }
        else {
            // Convert and print temperature and humidity
            printf("Temperature: %.1f°C, Humidity: %.1f%%\n", temp / 10.0, hum / 10.0);
        }
        
        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}