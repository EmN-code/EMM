#include <ztimer.h>
#include <stdio.h>

#define LED_PIN GPIO_PIN(0, 2)

static ztimer_t timer;

void timer_callback(void *arg) {
    (void)arg; // To avoid unused parameter warning
    // Code to turn on the LED
    printf("LED ON\n");
    gpio_set(LED_PIN);
}

int main(void) {
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Initialize the timer to trigger every 1 second
    ztimer_set(ZTIMER_MSEC, &timer, 1000);

    while (1) {
        // Main loop can perform other tasks
    }

    return 0;
}