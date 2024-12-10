
#include "periph/timer.h"  // Use periph/timer.h instead of ztimer.h
#include "event/callback.h"  // Use event/callback.h instead of event/periodic_callback.h
#include "thread.h"

#define TIMER_DEV         TIMER_DEV(0)
#define TIMER_CHANNEL     0
#define TIMER_INTERVAL    (1000000U)  // 1 second in microseconds

static void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void) {
    timer_init(TIMER_DEV, TIMER_INTERVAL, periodic_callback, NULL);
    timer_set(TIMER_DEV, TIMER_CHANNEL, TIMER_INTERVAL);

    while (1) {
        thread_sleep();  // Sleep to avoid busy-waiting
    }

    return 0;
}
