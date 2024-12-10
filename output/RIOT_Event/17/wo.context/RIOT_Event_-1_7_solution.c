#include <ztimer.h>
#include <stdio.h>
#include <msg.h>
#include <event.h>

#define INTERVAL (1U * US_PER_SEC)  // Set to 1 second

static ztimer_t timer;

static void _callback(void *arg)
{
    (void)arg;
    printf("Timer callback triggered\n");
}

int main(void)
{
    puts("Event Timer Example Started\n");

    // Initialize the ztimer
    ztimer_set_cb(ZTIMER_USEC, &timer, _callback, NULL);

    // Set timer interval and start periodic timer
    ztimer_periodic_wakeup(ZTIMER_USEC, 0, INTERVAL);

    // Infinite loop to keep the program running
    while (1) {
        // Temporarily sleep, allowing other processes
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
    }

    return 0;
}