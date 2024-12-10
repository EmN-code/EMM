
#include <stdio.h>
#include "thread.h"
#include "periph/gpio.h"
#include "xtimer.h"

#define DHT11_PIN         GPIO_PIN(0, 18)
#define LED_PIN           GPIO_PIN(0, 21)
#define TEMP_THRESHOLD    30
#define STACKSIZE         1024

static char thread_stack[STACKSIZE];

void read_dht11(void *arg) {
    (void)arg;
    while (1) {
        // Simulate reading temperature from DHT11
        int temperature = 35; // Replace with actual reading logic

        printf("Temperature: %d\n", temperature);

        if (temperature > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }

        xtimer_sleep(5);
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    thread_create(thread_stack, STACKSIZE, THREAD_PRIORITY_MAIN - 1, 0, read_dht11, NULL, "dht11_thread");

    return 0;
}
