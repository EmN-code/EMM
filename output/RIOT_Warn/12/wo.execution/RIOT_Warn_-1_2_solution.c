#include "thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "dht.h"
#include "event/thread.h"
#include <stdio.h>

#define DHT_PIN GPIO_PIN(0, 5)
#define LED_PIN GPIO_PIN(0, 2)
#define TEMP_THRESHOLD 25  // temperature threshold in Celsius

void *temperature_monitor(void *arg) {
    (void)arg;
    int16_t temp, hum;

    while (1) {
        if (dht_read(DHT_PIN, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temp / 10, temp % 10, hum / 10, hum % 10);
            
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }

        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    gpio_init(DHT_PIN, GPIO_IN);
    gpio_init(LED_PIN, GPIO_OUT);

    puts("DHT11 temperature monitoring started\n");

    static char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0,
                  temperature_monitor, NULL, "temp_monitor");

    return 0;
}