#include <ztimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define GPIO_PIN 2 // Define your GPIO pin number

void timer_callback(void *arg) {
    (void)arg; // Unused argument
    // Toggle the LED state
    if (!(gpio_read(GPIO_PIN))) {
        gpio_set(GPIO_PIN);
    } else {
        gpio_clear(GPIO_PIN);
    }
    printf("LED toggled\n");
}

int main(void) {
    ztimer_t timer;
    gpio_init(GPIO_PIN, GPIO_OUT);
    printf("Starting LED timer\n");

    // Set up the timer with a 1-second interval
    ztimer_set_msg(ZTIMER_MSEC, &timer, 1000, timer_callback, NULL);
    
    // The main thread can perform other tasks
    while(1) {
        // Sleep or perform other operations
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}