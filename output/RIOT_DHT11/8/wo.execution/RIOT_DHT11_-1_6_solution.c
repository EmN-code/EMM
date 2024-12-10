#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <msg.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <unistd.h>
#include <time_units.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define THREAD_STACKSIZE_MAIN (1024 * 4)  // Correct definition of THREAD_STACKSIZE_MAIN

// Define the GPIO pin for the DHT11 sensor
#define GPIO_PIN(x, y) ((x * 16) + y)
#define DHT11_PIN GPIO_PIN(0, 2)  // Correctly provided GPIO_PIN arguments

// Simulating the DHT11 functions
int dht_init(gpio_t pin, gpio_mode_t mode) {
    return gpio_init(pin, mode);
}

int dht_read(gpio_t pin, int16_t *temperature, int16_t *humidity) {
    *temperature = 25;  // Simulating a temperature reading
    *humidity = 50;     // Simulating a humidity reading
    return 0; // success
}

void *read_dht11(void *arg) {
    (void)arg;
    gpio_init(DHT11_PIN, 0);  // Initialize GPIO for the DHT11

    while (1) {
        int16_t temperature, humidity;
        if (dht_read(DHT11_PIN, &temperature, &humidity) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }
        ztimer_sleep(ZTIMER_SEC, 5);  // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    /* Create the thread to read DHT11 */
    char stack[THREAD_STACKSIZE_MAIN];
    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST, read_dht11, NULL, "read_dht11");
    if (pid <= KERNEL_PID_UNDEF) {
        printf("Error: failed to create thread\n");
        return 1;
    }

    return 0;
}