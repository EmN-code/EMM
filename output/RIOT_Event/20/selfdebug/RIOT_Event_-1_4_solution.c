#include <xtimer.h>
#include <stdio.h>

static void event_handler(void *arg)
{
    printf("Callback executed\n");
}

int main(void)
{
    ztimer_t timer;
    timer.callback = event_handler;  // Set the callback function
    timer.arg = NULL;  // No arguments for the callback

    while (1) {
        xtimer_set(&timer, 1 * US_PER_SEC);  // Set timer to 1 second interval
        ztimer_sleep(ZTIMER_USEC, 1 * US_PER_SEC);  // Sleep for a second to simulate periodic execution
    }

    return 0;
}