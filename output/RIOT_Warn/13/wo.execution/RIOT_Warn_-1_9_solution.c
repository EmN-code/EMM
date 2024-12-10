#include "ztimer.h"
#include "thread.h"
#include "stdio.h"
#include "dht.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "xtimer.h"

#define DHT_PIN 4
#define LED_PIN 2
#define TEMP_THRESHOLD 30

void* dht_read_thread(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;
    
    dev.type = DHT11;
    dev.pin = DHT_PIN;

    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read DHT sensor\n");
        }

        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    
    return NULL;
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, dht_read_thread, NULL, "dht_read");

    while (1) {
        xtimer_sleep(1);
    }
    
    return 0;
}