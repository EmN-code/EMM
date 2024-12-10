#include "xtimer.h"
#include "periph/gpio.h"

/* Define the GPIO port and pin for the LED */
#define LED_PORT GPIO_PIN(0, 2)  // Example for ESP32, GPIO0 pin 2

/* Timer interval: 1 second */
#define TIMER_INTERVAL_USEC (1U * US_PER_SEC)

static void toggle_led(void *arg) {
    (void)arg;
    gpio_toggle(LED_PORT);
}

int main(void) {
    /* Initialize LED pin */
    gpio_init(LED_PORT, GPIO_OUT);

    /* Timer setup */
    xtimer_t timer;
    timer.callback = toggle_led;
    timer.arg = NULL;
    xtimer_set(&timer, TIMER_INTERVAL_USEC);

    /* Let the timer run */
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}