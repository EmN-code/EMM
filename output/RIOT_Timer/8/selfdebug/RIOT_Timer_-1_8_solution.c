#include <ztimer.h>
#include <stdio.h>
#include <gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 5)  // Example GPIO_PIN, update according to your board's specifications
#define TIMER_INTERVAL (1U * ZTIMER_MSEC_PER_SEC)  // 1 second interval

static ztimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    // Toggle LED state
    gpio_toggle(LED_PIN);
    printf("LED Toggled!\n");
}

int main(void) {
    // Initialize the GPIO pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer with the TIMER_INTERVAL
    timer.callback = timer_callback;
    timer.arg = NULL;
    ztimer_set(ZTIMER_MSEC, &timer, TIMER_INTERVAL);

    printf("Timer setup complete.\n");

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL);
    }
    return 0;
}