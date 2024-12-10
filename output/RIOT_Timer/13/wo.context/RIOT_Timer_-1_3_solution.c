#include <ztimer.h>
#include <timex.h>
#include <periph/gpio.h>
#include <stdio.h>

#define LED_PIN GPIO_PIN(PORT_A, 5)

void timer_callback(void *arg) {
    (void)arg; // Unused argument
    gpio_set(LED_PIN);
    ztimer_sleep(ZTIMER_USEC, 1000000); // Sleep for 1 second
    gpio_clear(LED_PIN);
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    ztimer_t timer;
    timer_init(TIMER_DEV(0), 1, &timer_callback, NULL);

    while (1) {
        ztimer_periodic_wakeup(ZTIMER_USEC, 1000000);
    }
    return 0;
}