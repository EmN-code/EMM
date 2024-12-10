#include <ztimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define TIMER_INTERVAL (1U * US_PER_SEC)
#define LED_PIN GPIO_PIN(PORT_A, 0)   // Correct this with the appropriate port macro

static ztimer_t timer;

void callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    puts("Timer example
");

    // Initialize the LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    timer.callback = callback;
    timer.arg = NULL;

    // Set the timer
    ztimer_set(ZTIMER_USEC, &timer, TIMER_INTERVAL);

    return 0;
}