#include <timex.h>
#include <timer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 0)
#define INTERVAL (1U * US_PER_SEC)

void toggle_led(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);

    timer_t timer;
    timer.callback = toggle_led;
    timer.arg = NULL;

    xtimer_t xtimer;
    xtimer_init(&xtimer, INTERVAL, (xtimer_cb_t)toggle_led, NULL);
    xtimer_set(&xtimer);

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}