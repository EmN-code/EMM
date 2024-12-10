#include <ztimer.h>
#include <stdio.h>

void event_periodic_callback(void *arg)
{
    (void)arg;  // To handle unused parameter warning
    printf("Periodic event triggered\n");
}

int main(void)
{
    ztimer_t timer;
    /* Initialize timer */
    ztimer_init(ZTIMER_MSEC, &timer, (ztimer_callback_t)event_periodic_callback, NULL);
    /* Set timer to trigger every 1 second (1000 ms) */
    ztimer_set(ZTIMER_MSEC, &timer, 1000);

    while (1) {
        /* Process the timer to ensure regular callbacks */
        ztimer_sleep(ZTIMER_MSEC, 1000); // sleep for 1 second before next trigger
    }

    return 0;
}