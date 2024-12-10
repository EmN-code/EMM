
#include "periph/timer.h"
#include "periph/gpio.h"
#include "thread.h"
#include "xtimer.h"

#define TIMER_DEV         TIMER_DEV(0)
#define TIMER_CHANNEL     0
#define TIMER_INTERVAL    (1U * 1000U)  /* 1 second interval */

static void timer_callback(void *arg)
{
    (void)arg;
    gpio_toggle(GPIO_PIN(PORT_A, 5));  /* Toggle GPIO pin */
}

int main(void)
{
    /* Initialize GPIO pin */
    gpio_init(GPIO_PIN(PORT_A, 5), GPIO_OUT);

    /* Initialize timer */
    timer_init(TIMER_DEV, 1000000, timer_callback, NULL);

    /* Start the timer with the defined interval */
    timer_set_absolute(TIMER_DEV, TIMER_CHANNEL, xtimer_now_usec() + TIMER_INTERVAL);

    /* Main loop */
    while (1) {
        thread_yield();  /* Yield to other threads */
    }

    return 0;
}
