#include <ztimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_C, 8)

void timer_callback(void *arg) {
    (void)arg;
    gpio_set(LED_PIN);
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    timer.interval = 1 * US_PER_SEC;

    ztimer_set(ZTIMER_MSEC, &timer, timer.interval);

    while (1) {
        ztimer_sleep(ZTIMER_SEC, 1);
        gpio_clear(LED_PIN);
    }
    return 0;
}