#include <ztimer.h> 
#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define INTERVAL_USEC (1U * US_PER_SEC) // Timer interval: 1 second

ztimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    gpio_set(GPIO_PIN(0, 0)); // Assuming LED is connected to pin 0, level 0 (turns it on)
    printf("LED is turned on\n");
}

int main(void) {
    timer.callback = timer_callback;
    timer.arg = NULL;

    ztimer_set(ZTIMER_SEC, &timer, INTERVAL_USEC);

    while (1) {
        xtimer_sleep(1); // Sleep for 1 second to simulate main thread activity
        gpio_clear(GPIO_PIN(0, 0));
        xtimer_sleep(1); // Sleep for another second before turning the LED back on
    }

    return 0;
}