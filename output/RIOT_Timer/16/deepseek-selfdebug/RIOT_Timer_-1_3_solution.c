
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 13)  // Assuming GPIO0, pin 13 for the LED

static xtimer_t timer;

void toggle_led(void)
{
    static int state = 0;
    gpio_write(LED_PIN, state);
    state = !state;
}

int main(void)
{
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up the timer to trigger every second
    xtimer_init(&timer);
    xtimer_set(&timer, XTIMER_TICKS(1000000), toggle_led);

    // Main loop (not strictly necessary in this case, but good practice)
    while (1) {
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
