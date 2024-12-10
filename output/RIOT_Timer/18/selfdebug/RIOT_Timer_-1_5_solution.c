#include "xtimer.h"
#include "ztimer.h"
#include "periph/gpio.h"
#include <stdio.h>

#define LED_PIN         GPIO_PIN(PORT_A, 5)
#define TIMER_INTERVAL  (1U * 1000000U)  // 1 second in microseconds

static void timer_callback(void *arg)
{
    (void)arg;
    // Toggle LED
    gpio_toggle(LED_PIN);
}

int main(void)
{
    // Initialize LED pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error to initialize LED pin\n");
        return 1;
    }

    // Initialize timer
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    xtimer_set(&timer, TIMER_INTERVAL);

    // Main loop does nothing, just wait for interrupt
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}