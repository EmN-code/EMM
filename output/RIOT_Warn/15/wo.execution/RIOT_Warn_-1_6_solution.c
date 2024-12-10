#include "thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "dht_params.h"
#include "event/thread.h"
#include "mma8x5x.h"
#include "dht.h"

#define TEMP_THRESHOLD 30  // Temperature threshold in °C
#define GPIO_PIN 2         // GPIO pin for the LED

void toggle_led(int high) {
    if (high) {
        gpio_set(GPIO_PIN);
    } else {
        gpio_clear(GPIO_PIN);
    }
}

void *dht_reader_thread(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temperature, humidity;

    gpio_init(GPIO_PIN, GPIO_OUT);

    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
            if (temperature > TEMP_THRESHOLD) {
                toggle_led(1);
            } else {
                toggle_led(0);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, 0,
                  dht_reader_thread, NULL, "dht_reader");

    // Main thread has nothing to do
    while (1) {
        thread_sleep();
    }
    return 0;
}