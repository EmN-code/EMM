
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define STACKSIZE 512

static char stack[STACKSIZE];
static event_queue_t event_queue;

void periodic_event_callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    event_thread_init(&event_queue, stack, STACKSIZE, PRIORITY_MAIN - 1);

    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, xtimer_ticks32(xtimer_s_to_usec(1)), periodic_event_callback, NULL);

    event_add_periodic(&event_queue, &periodic_event);

    return 0;
}
