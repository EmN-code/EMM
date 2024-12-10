#include "thread.h"
#include "event/thread.h"
#include "ztimer.h"

#define INTERVAL (1U * ZTIMER_MSEC_PER_SEC) /* 1 second interval */

static ztimer_periodic_t timer;

void callback(void *arg) {
    /* Callback function code here. */
    (void)arg;  // To avoid unused variable warning
    printf("Periodic callback executed!\n");
}

int main(void) {
    /* Initialize periodic timer */
    ztimer_periodic_init(ZTIMER_MSEC, &timer, callback, NULL, INTERVAL);

    /* Start the timer */
    ztimer_periodic_start(&timer);

    /* Let the main thread yield when no other process is scheduled */
    while (1) {
        thread_yield();
    }

    return 0;
}