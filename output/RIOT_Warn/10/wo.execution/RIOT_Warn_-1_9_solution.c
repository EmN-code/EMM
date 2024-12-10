#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "thread.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "periph/gpio.h"
#include "msg.h"
#include "mm24x5x_regs.h"
#include "mm8x5x_params.h"
#include "mm8x5x.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_system.h"
#include "dht_params.h"
#include "dht.h"  // Ensure this custom header defines DHT_TYPE_DHT11

#define DHT_PIN GPIO_PIN(PORT_0, 1) // Modify with actual port
#define LED_PIN GPIO_PIN(PORT_0, 2) // Update to actual port
#define TEMP_THRESHOLD 25

static char stack[THREAD_STACKSIZE_MAIN];

void *dht_thread(void *arg) {
    dht_t dev;
    dev.pin = DHT_PIN;
    dev.type = DHT_TYPE_DHT11;

    while (1) {
        int16_t temperature, humidity;
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            puts("Failed to read from DHT sensor");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    puts("Starting DHT reader thread...");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, dht_thread, NULL, "dht_reader");
    return 0;
}