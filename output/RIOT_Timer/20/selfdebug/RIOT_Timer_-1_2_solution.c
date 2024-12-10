#include <ztimer.h>
#include <stdio.h>
#include "periph/gpio.h"

#define LED0_PIN GPIO_PIN(PORT_C, 8)

static void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED0_PIN);
    printf("Timer triggered! LED toggled.\n");
}

int main(void) {
    printf("Starting the timer example...\n");

    if (gpio_init(LED0_PIN, GPIO_OUT)) {
        printf("Error to initialize LED pin\n");
        return 1;
    }

    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    ztimer_set(ZTIMER_MSEC, &timer, 1000);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}
