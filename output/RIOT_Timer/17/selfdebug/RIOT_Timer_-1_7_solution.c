#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 0)
#define TIMER_INTERVAL (1U * US_PER_SEC)

static void timer_callback(void *arg)
{
    (void)arg;
    // Toggle LED state
    gpio_toggle(LED_PIN);
    printf("LED toggled\n");
}

int main(void)
{
    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize timer
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    xtimer_set(&timer, TIMER_INTERVAL);

    printf("Timer setup complete. LED will toggle every second.\n");

    // Main loop does nothing, everything is handled by the timer
    while (1) {
        // Sleep to save power
        xtimer_sleep(1);
    }

    return 0;
}