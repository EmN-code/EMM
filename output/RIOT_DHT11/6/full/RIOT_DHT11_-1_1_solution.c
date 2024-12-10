#include "xtimer.h"
#include "timex.h"
#include "dht.h"
#include "thread.h"
#include "stdio.h"
#include "periph/gpio.h"

#define SLEEP_INTERVAL (5U * US_PER_SEC)  // 5 seconds
#define DHT_PIN GPIO_PIN(0, 2)           // GPIO pin 2

int main(void) {
    // Initialize the DHT sensor
    dht_t dev;
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;
    if (dht_init(&dev, &params) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return -1;
    }
    
    int16_t temp, hum;
    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read DHT sensor\n");
        }
        
        // Sleep for the specified interval
        xtimer_usleep(SLEEP_INTERVAL);
    }
    return 0;
}