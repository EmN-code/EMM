#include "ztimer.h"
#include "periph/gpio.h"
#include "dht.h"

#define TEMP_SENSOR_PIN GPIO_PIN(0, 2)
#define SLEEP_DURATION (5U * ZTIMER_USEC_PER_SEC) // Use ZTIMER_USEC_PER_SEC instead of US_PER_SEC

void *dht11_thread(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;

    dht_init(&dev, TEMP_SENSOR_PIN, DHT11);

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%dC, Humidity: %d.%d%%\n", 
                    temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read DHT11 sensor\n");
        }
        ztimer_sleep(ZTIMER_MSEC, SLEEP_DURATION);
    }
    return NULL;
}

int main(void) {
    puts("Starting DHT11 example...");
    dht11_thread(NULL);
    return 0;
}