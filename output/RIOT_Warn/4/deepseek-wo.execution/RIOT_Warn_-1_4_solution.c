
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include "periph/gpio.h"
#include "thread.h"
#include "xtimer.h"

#define DHT11_PIN GPIO_PIN(0, 18)
#define LED_PIN GPIO_PIN(0, 2)
#define TEMP_THRESHOLD 30

static char dht_thread_stack[THREAD_STACKSIZE_DEFAULT];

void dht_thread(void *arg) {
    (void)arg;
    while (1) {
        // Simulate reading temperature from DHT11
        int temperature = 35; // Simulated temperature value

        printf("Temperature: %d\n", temperature);

        if (temperature > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }

        xtimer_sleep(5); // Sleep for 5 seconds
    }
}

int main(void) {
    // Initialize GPIO pins
    gpio_init(DHT11_PIN, GPIO_IN);
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a thread to handle DHT11 reading and LED control
    thread_create(dht_thread_stack, sizeof(dht_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, 0,
                  (thread_task_func_t)dht_thread, NULL, "dht_thread");

    return 0;
}
