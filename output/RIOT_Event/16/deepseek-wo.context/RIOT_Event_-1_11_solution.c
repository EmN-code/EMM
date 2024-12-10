
#include "xtimer.h"
#include "event/callback.h"
#include "event/periodic.h"

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE)

static void periodic_callback(void *arg) {
    printf("Periodic callback triggered\n");
}

int main(void) {
    event_periodic_t periodic_event;
    char stack[STACK_SIZE];

    event_periodic_callback_init(&periodic_event, xtimer_ticks_from_usec(1000000), periodic_callback, NULL);
    event_periodic_start(&periodic_event);

    return 0;
}
