
#include "event/periodic.h"
#include "thread.h"
#include "xtimer.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACKSIZE];
static event_queue_t queue;

void callback(void *arg) {
    printf("Periodic event triggered!\n");
}

int main(void) {
    // Initialize the event queue
    event_thread_init(&queue, stack, sizeof(stack), PRIORITY);

    // Initialize the periodic event
    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, &queue, callback, NULL, 1000000); // 1 second interval

    // Start the periodic event
    event_periodic_start(&periodic_event);

    // Keep the main thread alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
