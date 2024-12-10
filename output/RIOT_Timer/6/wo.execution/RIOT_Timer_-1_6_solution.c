#include <ztimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2) // assuming GPIO_PIN(0,2) is used

void timer_callback(void *arg) {
    (void)arg;
    // Toggle LED state
    gpio_toggle(LED_PIN);
}

int main(void) {
    printf("RIOT LED Timer example
");
    
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Initialize a timer
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.interval = 1000000; // 1 second in microsecond
    timer.arg = NULL;

    // Start the timer
    ztimer_set(ZTIMER_MSEC, &timer, timer.interval);

    while (1) {
        // Main loop does nothing, just waiting for timer callback
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}