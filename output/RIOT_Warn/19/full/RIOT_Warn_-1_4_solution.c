#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <periph/dht.h>

#define DHT_PORT    GPIO_PIN(PORT_A, 5)
#define LED_PORT    GPIO_PIN(PORT_B, 0)
#define TEMP_THRESHOLD 25
#define SLEEP_INTERVAL (5U * US_PER_SEC)

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    int rc;

    rc = dht_init(&dev, DHT_PORT);
    if (rc != DHT_OK) {
        printf("DHT initialization error\n");
        return 1;
    }

    gpio_init(LED_PORT, GPIO_OUT);

    while (1) {
        rc = dht_read(&dev, &temp, &hum);
        if (rc == DHT_OK) {
            printf("Temperature: %d.%d°C - Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp >= TEMP_THRESHOLD * 10) {
                gpio_set(LED_PORT);
            } else {
                gpio_clear(LED_PORT);
            }
        } else {
            printf("Error reading DHT sensor\n");
        }

        xtimer_sleep(SLEEP_INTERVAL / US_PER_SEC);
    }
    return 0;
}