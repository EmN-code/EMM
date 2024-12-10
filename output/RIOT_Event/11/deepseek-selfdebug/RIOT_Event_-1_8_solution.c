
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <event/callback.h>

#define THREAD_STACKSIZE_DEFAULT (512)
#define THREAD_PRIORITY_MAIN (THREAD_PRIORITY_MAIN)

static void event_periodic_callback(void *arg) {
    (void)arg;
    printf("Event periodic callback triggered!\n");
}

int main(void) {
    event_callback_t event_callback;
    event_callback.handler = event_periodic_callback;
    event_callback.arg = NULL;

    event_thread_init(&event_callback, NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN);

    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, 1 * US_PER_SEC);
        event_post(&event_callback);
    }

    return 0;
}
