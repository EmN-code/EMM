#include <xtimer.h>  // To handle sleep functionality
#include "thread_arch.h"  // Thread architecture for RTOS
#include "periph_conf.h"  // Peripheral configuration
#include "dht.h"  // DHT sensor library

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO2 corresponds to pin 0,2
#define SLEEP_INTERVAL 5000000  // 5 seconds in microseconds

int main(void) {
    printf("DHT11 Reader Example\n");

    // Define and initialize DHT sensor
    dht_t dev;
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;
    int res = dht_init(&dev, &params);

    while (res != 0) {
        printf("Failed to initialize DHT sensor\n");
        xtimer_sleep(1);  // Retry every second if initialization failed
        res = dht_init(&dev, &params);
    }

    // Main reading loop
    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        xtimer_usleep(SLEEP_INTERVAL);  // Wait for 5 seconds
    }

    return 0;
}