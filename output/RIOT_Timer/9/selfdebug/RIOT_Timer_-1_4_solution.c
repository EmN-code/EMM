#include <xtimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2)
#define INTERVAL (1U * US_PER_SEC)  // 1 second interval

static xtimer_t timer;

static void toggle_led(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the GPIO pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the timer
    xtimer_init();
    timer.callback = toggle_led;
    timer.arg = NULL;

    while (1) {
        xtimer_set(&timer, INTERVAL);
    }

    return 0;
}