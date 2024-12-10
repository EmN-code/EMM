#include <stdio.h>
#include <dht.h>
#include <xtimer.h>
#include <periph/gpio.h>

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO pin 2
#define INTERVAL (5U * 1000000U)  // 5 seconds

int main(void)
{
    int16_t temp, hum;
    dht_t dev;
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;

    if (dht_init(&dev, &params) != DHT_OK) {
        printf("DHT initialization failed\n");
        return 1;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            printf("DHT read failed\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp/10, temp%10, hum/10, hum%10);
        }
        xtimer_sleep(INTERVAL/1000000);  // Convert microseconds to seconds
    }

    return 0;
}