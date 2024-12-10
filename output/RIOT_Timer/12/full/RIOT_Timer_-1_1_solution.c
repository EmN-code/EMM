#include "timers.h"
#include "thread.h"
#include "periph_conf.h"
#include "nimble_host.h"
#include "gpio.h"
#include "board.h"
#include "stdio.h"

#define LED_PIN LED0_PIN

timer_t timer;

void timer_callback(void *arg) {
    (void)arg; // suppress unused parameter warning
    // Toggle the LED state
    gpio_toggle(LED_PIN);
    printf("LED Toggled\n");
}

int main(void) {
    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);
    printf("Starting the timer\n");

    // Set up the timer to trigger every 1 second
    if (timer_init(&timer, 1 * 1000000, timer_callback, NULL) != 0) {
        printf("Failed to initialize timer\n");
        return -1;
    }

    timer_start(timer);

    while (1) {
        thread_sleep();
    }

    return 0;
}