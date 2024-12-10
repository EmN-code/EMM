#include "xtimer.h"
#include "periph/gpio.h"
#include "fmt.h"

#define LED_PIN GPIO_PIN(0, 2) // This should be set according to your board's pin configuration
#define DELAY_SEC (1U) // Delay in seconds

static void cb(void *arg) {
    (void)arg;
    static bool led_state = false;
    // Toggle LED
    if (led_state) {
        gpio_clear(LED_PIN);
    }
    else {
        gpio_set(LED_PIN);
    }
    // Change state
    led_state = !led_state;
}

int main(void) {
    // Initialize LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error to initialize GPIO\n");
        return 1;
    }

    // Setup timer
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        cb(NULL);
        xtimer_periodic_wakeup(&last_wakeup, DELAY_SEC * US_PER_SEC);
    }

    return 0;
}