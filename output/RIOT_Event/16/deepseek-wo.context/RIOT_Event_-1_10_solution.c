
#include "xtimer.h"
#include "event/periodic.h"
#include "event/callback.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT + DEBUG_EXTRA_STACKSIZE)

static char stack[STACKSIZE];
static event_queue_t event_queue;

void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_queue_init(&event_queue);

    event_periodic_t periodic_event;
    event_periodic_callback_init(&periodic_event, xtimer_ticks64(1 * US_PER_SEC), periodic_callback, NULL);

    event_periodic_start(&periodic_event, &event_queue);

    while (1) {
        event_loop(&event_queue);
    }

    return 0;
}
