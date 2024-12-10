#include <stdio.h>
#include <event/callback.h>
#include "timex.h"
#include "ztimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(PORT_A, 5)  // Define the GPIO pin connected to the LED

// Callback function for the timer
static void timer_callback(void *arg) {
    (void)arg;
    // Toggle LED state
    gpio_tgl(LED_PIN);
    printf("LED toggled!\n");
}

int main(void) {
    // Initialize the GPIO pin connected to the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Create and initialize a ztimer
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.args = NULL;
    
    // Set the timer to trigger every second
    ztimer_set_ext(ZTIMER_SEC, &timer, 1);

    // Enter an infinite loop to keep the program running
    while (1) {
        // Power save when no interrupts
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}