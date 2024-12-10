
#include "xtimer.h"
#include "periph/gpio.h"
#include "dht11.h"

#define DHT11_PIN GPIO_PIN(0, 2)

int main(void)
{
    dht11_t dev;
    int16_t temp, hum;

    if (dht11_init(&dev, DHT11_PIN) == DHT11_OK) {
        while (1) {
            if (dht11_read(&dev, &temp, &hum) == DHT11_OK) {
                printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                       temp / 10, temp % 10, hum / 10, hum % 10);
            } else {
                printf("Failed to read from DHT11 sensor\n");
            }
            xtimer_sleep(5);
        }
    } else {
        printf("Failed to initialize DHT11 sensor\n");
    }

    return 0;
}
