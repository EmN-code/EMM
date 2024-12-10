#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO 2 for ESP32
#define DHT_TYPE DHT_TYPE_DHT11
#define SLEEP_TIME 5

int main(void)
{
    int16_t temp, hum;
    xtimer_ticks32_t last_wakeup = xtimer_now();

    printf("Initialize DHT sensor on GPIO 2\n");

    // Initialize DHT sensor (Note: dht_init() generally does not require many arguments)
    while (1) {
        if (dht_init(DHT_PIN) != 0) {
            printf("Failed to initialize DHT sensor\n");
            xtimer_sleep(1);  // Retry every second
            continue;
        } else {
            printf("DHT sensor initialized successfully\n");
            break;
        }
    }

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, SLEEP_TIME * US_PER_SEC);

        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }
    }

    return 0;
}