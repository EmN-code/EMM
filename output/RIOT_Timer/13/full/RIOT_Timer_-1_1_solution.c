#include "ztimer.h"
#include "periph/gpio.h"
#include "board.h"
#include <stdio.h>

#ifndef GPIO_PIN
#define GPIO_PIN GPIO_PIN(0, 2)  // Example GPIO pin, ensure the board has this configured
#endif

static void cb(void *arg) {
    (void)arg;
    /* Toggle LED state */
    gpio_toggle(GPIO_PIN);
}

int main(void) {
    /* Initialize GPIO for LED */
    if (gpio_init(GPIO_PIN, GPIO_OUT)) {
        puts("Error to initialize GPIO");
        return 1;
    }

    /* Create ztimer instance with 1s interval */
    ztimer_now_t last_wakeup = ztimer_now();
    while (1) {
        ztimer_sleep(ZTIMER_USEC, 1000000);
        /* Call interrupt service routine equivalent */
        cb(NULL);

        /* Re-schedule timer */
        last_wakeup += 1000000;
    }

    return 0;
}