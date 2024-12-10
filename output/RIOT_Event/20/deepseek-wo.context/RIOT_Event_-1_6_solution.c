
#include "xtimer.h"
#include "thread.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void event_periodic_callback(void *arg)
{
    (void)arg;  // Unused argument
    printf("Event periodic callback triggered!\n");
}

int main(void)
{
    xtimer_t timer;
    uint32_t last_wakeup = xtimer_now().ticks32;

    // Initialize the timer and set the callback function
    timer.callback = event_periodic_callback;
    timer.arg = NULL;

    // Start the periodic timer
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
        timer.callback(timer.arg);
    }

    return 0;
}
