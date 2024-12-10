#include <ztimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2) // Change to the correct pin for your setup

static void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
    printf("LED toggled\n");
}

int main(void) {
    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timer
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    ztimer_set(&timer, ztimer_usec_from_sec(1)); // Set timer to trigger every 1 second

    // Main loop
    while (1) {
        ztimer_sleep(ZTIMER_USEC, 1000000);
    }

    return 0;
}