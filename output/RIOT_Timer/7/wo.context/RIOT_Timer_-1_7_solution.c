#include <xtimer.h>
#include <thread.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2) // Change as per your board's LED pin
#define TIMER_INTERVAL (1U * US_PER_SEC)

static void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    static xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    xtimer_set(&timer, TIMER_INTERVAL);

    while (1) {
        thread_yield();
    }

    return 0;
}