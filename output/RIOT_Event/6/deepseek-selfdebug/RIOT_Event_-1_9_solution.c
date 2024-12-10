
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACKSIZE];

static void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    event_queue_t event_queue;
    event_t event;

    event_queue_init(&event_queue);
    event_init(&event, periodic_callback, NULL);

    xtimer_t timer;
    xtimer_init(&timer);
    xtimer_set_periodic(&timer, XTIMER_TICKS(1000000), (xtimer_callback_t)event_post, &event_queue, &event);

    while (1) {
        event_process(&event_queue);
    }

    return 0;
}
