#include <ztimer.h>
#include <stdio.h>
#include <event.h>

/* Callback function for the event */
void event_periodic(void *arg)
{
    (void)arg;  // suppress unused variable warning
    printf("Event triggered every second.\n");
}

int main(void)
{
    /* Initialize a ztimer instance */
    ztimer_t timer;
    /* Specify the duration for the timer of 1 second */
    uint32_t timeout = 1 * ZTIMER_MSEC;  // ZTIMER_MSEC is typically correct for millisecond granularity

    /* Configure the timer to call the callback function every second */
    ztimer_set(&ztimer_msec, &timer, timeout, event_periodic, NULL);  // Ensure the event_periodic argument is a proper function pointer

    /* Start the event loop, if applicable */
    while (1) {
        /* You can add other tasks or simply keep the system active */
    }

    return 0;
}