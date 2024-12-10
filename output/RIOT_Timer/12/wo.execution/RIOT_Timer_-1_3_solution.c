#include <ztimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2)
#define INTERVAL ZTIMER_MSEC(1000)
#define TIMER_STACKSIZE (THREAD_STACKSIZE_MAIN)

static char timer_stack[TIMER_STACKSIZE];

void timer_cb(void *arg){
    (void)arg; // Avoid unused parameter warning
    gpio_set(LED_PIN);
    puts("LED on");
}

int main(void){
    // Set pin mode to output for LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize and start the timer
    ztimer_t timer;
    ztimer_set_callback(ZTIMER_MSEC, &timer, timer_cb, NULL);
    ztimer_start_periodic(ZTIMER_MSEC, &timer, INTERVAL);

    return 0;
}