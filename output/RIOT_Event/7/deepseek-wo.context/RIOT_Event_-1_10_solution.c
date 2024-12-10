
#include "xtimer.h"
#include "thread.h"
#include "event/callback.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACKSIZE];
static event_queue_t event_queue;

static void event_periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    // Initialize the event queue
    event_thread_init(&event_queue, stack, STACKSIZE, THREAD_PRIORITY_MAIN - 1);

    // Create a periodic event
    xtimer_t periodic_timer;
    periodic_timer.callback = (xtimer_callback_t)event_periodic_callback;
    periodic_timer.arg = NULL;

    // Set the periodic timer to trigger every second
    xtimer_set_periodic(&periodic_timer, 1 * US_PER_SEC);

    // Keep the main thread alive
    while (1) {
        thread_yield();
    }

    return 0;
}
