#include <timex.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <xtimer.h>

#define INTERVAL (1U * US_PER_SEC)  // 1 second interval
#define LED_PIN GPIO_PIN(PORT_A, 2) // Assuming LED is connected to PA2

static xtimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
    printf("LED Toggled\n");
}

int main(void) {
    // Initialize the GPIO pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer
    xtimer_set(&timer, INTERVAL);
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Continue looping
    while (1) {
        // Let the timer invoke the callback
        xtimer_usleep(INTERVAL);
    }

    return 0;
}