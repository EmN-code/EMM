#include "xtimer.h"
#include "timex.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT11_TYPE DHT11
#define DHT11_PIN GPIO_PIN(0, 2) // GPIO2

int main(void) {
    dht_t dev;
    dht_params_t params;
    int16_t temperature, humidity;

    // Initialize the parameters for DHT
    params.pin = DHT11_PIN;
    params.type = DHT11_TYPE;

    // Initialize the DHT device
    if (dht_init(&dev, &params) != 0) {
        printf("DHT initialization failed\n");
        return 1;
    }

    // Main loop
    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            // Print the results
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
        } else {
            printf("Failed to read DHT11 sensor\n");
        }
        
        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}