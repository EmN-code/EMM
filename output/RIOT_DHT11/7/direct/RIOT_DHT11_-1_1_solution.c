#include "xtimer.h"
#include "dht.h"
#include "dht_params.h"
#include "periph/gpio.h"

// Allocate memory for DHT11 parameters
static dht_params_t dht_params = {
    .type = DHT11,
    .pin = GPIO2,
    .negative_logic = false
};

int main(void) {
    // This will hold temperature and humidity values read from the DHT11
    int16_t temp, hum;
    
    // Initialize DHT11 sensor
    dht_init(&dht_params);

    while(1) {
        // Give some time for the sensor to stabilize
        xtimer_sleep(DHT11_WAKEUP_INTERVAL);

        // Read the sensor values
        if (dht_read(&dht_params, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            puts("Error reading from DHT11 sensor\n");
        }
        
        // Sleep for 5 seconds before reading again
        xtimer_sleep(5);
    }
    return 0;
}