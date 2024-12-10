#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "thread.h"
#include "ztimer.h"
#include "ztimer/periodic.h"
#include "ztimer/timer.h"
#include <stdio.h>

#define DHT_PIN GPIO_PIN(0, 4)  // DHT11 is connected to GPIO4
#define LED_PIN GPIO_PIN(0, 5)  // LED is connected to GPIO5
#define TEMP_THRESHOLD 30       // Temperature threshold in Celsius

void *dht_thread(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;

    if (dht_init(&dev, &dht_params[0]) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return NULL;
    }

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temp: %d.%d°C, Hum: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT11\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return NULL;
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    thread_create(dht_thread_stack, sizeof(dht_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, 0, dht_thread, NULL, "dht_thread");

    return 0;
}