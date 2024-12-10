#include "xtimer.h"
#include "scu.h"
#include "periph/gpio.h"
#include <stdio.h>

#define LED  GPIO_PIN(0, 2)

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED);
}

int main(void) {
    gpio_init(LED, GPIO_OUT);
    xtimer_ticks32_t last_wakeup = xtimer_now();
    
    while (1) {
        timer_callback(NULL);
        xtimer_periodic_wakeup(&last_wakeup, XTIMER_SEC(1));
    }
    return 0;
}