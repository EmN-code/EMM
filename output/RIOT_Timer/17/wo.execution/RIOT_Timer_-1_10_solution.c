#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdint.h"
#include "periph/gpio.h"
#include "timex.h"
#include "ztimer.h"

#define INTERVAL 1U    // Timer interval in seconds
#define LED_PIN GPIO_PIN(0, 2)   // Example for an ESP32 GPIO pin

static void _timer_callback(void *arg) {
    (void)arg; // Unused
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the GPIO pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error to initialize GPIO pin\n");
        return 1;
    }

    // Initialize ztimer
    ztimer_t timer;
    ztimer_set(&timer, ZTIMER_SEC, INTERVAL, _timer_callback, NULL);

    // Start the timer
    while (1) {
        ztimer_sleep(ZTIMER_SEC, INTERVAL);
    }

    return 0;
}