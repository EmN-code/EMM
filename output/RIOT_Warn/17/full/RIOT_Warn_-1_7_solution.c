#include <xtimer.h>
#include <periph/gpio.h>
#include "dht.h"
#include "dht_params.h"

#define DHT_TYPE    DHT11
#define DHT_PIN     GPIO_PIN(0, 4) // Example GPIO pin, change as necessary
#define LED_PIN     GPIO_PIN(0, 5) // Example GPIO pin for LED
#define TEMP_THRESHOLD 30
#define READ_INTERVAL (5U * US_PER_SEC)

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    int rc;

    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        rc = dht_init(&dev, &dht_params[0]);
        if (rc != DHT_OK) {
            printf("DHT initialization failed\n");
            xtimer_sleep(1);
            continue;
        }

        rc = dht_read(&dev, &temp, &hum);
        if (rc != DHT_OK) {
            printf("DHT read failed\n");
            xtimer_sleep(1);
            continue;
        }

        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
               temp / 10, temp % 10, hum / 10, hum % 10);

        if (temp / 10 > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }

        xtimer_usleep(READ_INTERVAL);
    }

    return 0;
}