#include "xtimer.h"
#include "timex.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(PORT_A, 0)
#define INTERVAL (1U * US_PER_SEC)

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);

    // Timer setup
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    
    while (1) {
        // Wait for the next timer interval
        xtimer_set(&timer, INTERVAL);
        xtimer_usleep(INTERVAL);
    }

    return 0;
}