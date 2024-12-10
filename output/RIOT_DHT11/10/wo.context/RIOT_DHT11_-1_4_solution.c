#include "xtimer.h"
#include "timex.h"
#include "dht.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 2)

int main(void) {
    dht_t dev;
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;
    dht_init(&dev, &params); // Correct function signature

    int16_t temp, hum;

    while (1) {
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Error reading sensor data\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        }
        xtimer_sleep(5);
    }

    return 0;
}