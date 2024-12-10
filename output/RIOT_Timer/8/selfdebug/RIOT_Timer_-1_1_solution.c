#include <stdio.h>
#include <timex.h>
#include <ztimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2)  // Assumed LED pin, change as necessary

static ztimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    // Toggle LED state
    gpio_toggle(LED_PIN);
    // For debugging purposes
    printf("LED toggled\n");
}

int main(void) {
    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up the ztimer
    ztimer_set(ZTIMER_SEC, &timer, 1, timer_callback, NULL);
    
    // Keep the main function alive
    while (1) {
        // Do nothing, ztimer will handle the LED toggling
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}