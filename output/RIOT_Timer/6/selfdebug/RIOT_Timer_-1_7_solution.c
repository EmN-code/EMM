#include <xtimer.h>
#include <periph/gpio.h>
#include <event.h>

#define LED_PIN GPIO_PIN(PORT_A, 5)  // assuming PORT_A and pin 5, customize as per your setup

// Function to toggle LED
void toggle_led(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void)
{
    // Initialize LED_PIN as output
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        // Error handling
        return 1;
    }

    // Timer configuration
    static xtimer_t timer;
    timer.callback = toggle_led;
    timer.arg = NULL;

    // Set timer for periodic wakeups every second
    while (1) {
        xtimer_set(&timer, 1 * XTIMER_USEC_PER_SEC);
        xtimer_sleep(1);
    }

    return 0;
}