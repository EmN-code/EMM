#include "xtimer.h"
#include "stdio.h"

#define TIMER_INTERVAL (1U * US_PER_SEC)

static void timer_callback(void *arg)
{
    (void)arg;
    printf("Timer callback function triggered\n");
}

int main(void)
{
    xtimer_t timer;

    /* Initialize the timer */
    xtimer_set(&timer, TIMER_INTERVAL);

    /* Set the callback function for the timer */
    timer.callback = timer_callback;
    timer.arg = NULL;

    /* Start the timer */
    while (1) {
        xtimer_spin_until(DEBUG_PIN,
                          timer.target);
        /* Call the timer callback */
        timer_callback(timer.arg);
    }

    return 0;
}