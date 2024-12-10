#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)  // Define the GPIO pin where DHT11 is connected
#define SLEEP_DURATION (5U * 1000000U)  // 5 seconds in microseconds

int main(void) {
    dht_params_t dev = {
        .pin = DHT_PIN,
        .type = DHT11
    };
    int16_t temp, hum;
    dht_t dev_handle;
    
    if (dht_init(&dev_handle, &dev) != DHT_OK) {
        puts("DHT initialization failed
");
        return 1;
    }

    while (1) {
        if (dht_read(&dev_handle, &temp, &hum) != DHT_OK) {
            puts("Failed to read data from DHT11 sensor
");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        }
        xtimer_usleep(SLEEP_DURATION);
    }
    return 0;
}