#include "xtimer.h"
#include "timex.h"
#include "periph/gpio.h"

#define TIMER_INTERVAL (1U * US_PER_SEC)
#define LED_PIN        GPIO_PIN(0, 4)  // Ensure correct pin configuration

timer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    gpio_set(LED_PIN);
}

int main(void) {
    // Initialize GPIO
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize timer
    timer.callback = timer_callback;
    timer.arg = NULL;
    
    while (1) {
        xtimer_set(&timer, TIMER_INTERVAL);
        gpio_clear(LED_PIN);  // Ensure correct LED state toggle
        xtimer_usleep(TIMER_INTERVAL);
    }

    return 0;
}