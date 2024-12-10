#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)
#define INTERVAL (5U * US_PER_SEC) // 5 seconds interval

int main(void)
{
    gpio_t pin = DHT_PIN;
    int16_t temp, hum;

    while (1) {
        if (dht_read(&dht_params[DHT11], pin, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from sensor\n");
        }
        xtimer_sleep(5);
    }
    return 0;
}