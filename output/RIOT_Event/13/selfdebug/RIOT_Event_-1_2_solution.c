#include <ztimer.h>
#include <stdio.h>
#include <thread.h>

void callback(void *arg) {
    /* Callback function for the timer event */
    printf("Timer triggered!\n");
}

int main(void) {
    ztimer_periodic_t periodic_timer;
    /* Initialize the periodic timer to trigger every second (1000ms) */
    ztimer_periodic_init(ZTIMER_SEC, &periodic_timer, callback, NULL, 1);

    /* Start the periodic timer */
    ztimer_periodic_start(&periodic_timer);

    while (1) {
        /* Keep the main thread alive */
        ztimer_sleep(ZTIMER_SEC, 1);
    }
    return 0;
}