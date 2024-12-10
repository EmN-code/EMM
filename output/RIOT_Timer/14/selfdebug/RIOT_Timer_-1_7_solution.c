#include <ztimer.h>

#define TIMER_INTERVAL (1U * ZTIMER_SEC) // Timer interval of 1 second

static void led_on(void *arg) {
    // Function to turn on the LED
    (void)arg; // avoid unused parameter warning
    // Here, you should add code to turn on your specific LED
    printf("LED turned on\n");
}

int main(void) {
    ztimer_t timer;
    timer.callback = led_on;
    ztimer_set(ZTIMER_MSEC, &timer, TIMER_INTERVAL);
    
    while (1) {
        ztimer_sleep(ZTIMER_SEC, TIMER_INTERVAL);
    }

    return 0;
}