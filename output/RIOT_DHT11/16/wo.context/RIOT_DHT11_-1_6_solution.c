#include <thread.h>
#include <timex.h>
#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#include "dht.h"
#include "dht_params.h"

#define DHT11_TYPE DHT11
#define DHT11_PIN GPIO_PIN(0, 2) // GPIO 2 on ESP32
#define INTERVAL (5U * US_PER_SEC) // 5 seconds interval

static dht_t dev;

int main(void) {
    if (dht_init(&dev, &dht_params[0]) != DHT_OK) {
        puts("Failed to initialize DHT11 sensor");
        return 1;
    }
    
    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) != DHT_OK) {
            puts("Error reading DHT11 sensor");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        }
        xtimer_sleep(INTERVAL);
    }

    return 0;
}