#include <ztimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2) // Adjust based on your hardware

static ztimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    // Toggle the LED state
    gpio_toggle(LED_PIN);
    // Rearm timer to fire again after 1 second
    ztimer_set(ZTIMER_SEC, &timer, 1);
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the ztimer structure
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set the first timer event 1 second from now
    ztimer_set(ZTIMER_SEC, &timer, 1);

    // Main loop:
    while (1) {
        // The MCU can sleep, waiting for interrupts
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}