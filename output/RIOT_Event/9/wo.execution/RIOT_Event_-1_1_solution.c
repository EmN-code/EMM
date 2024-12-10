#include "thread.h"
#include "event/periodic.h"
#include "event/callback.h"

static int _callback_counter = 0;

void my_periodic_callback(void *arg) {
    (void)arg; // Unused
    _callback_counter++;
    printf("Periodic callback executed %d times\n", _callback_counter);
}

int main(void) {
    printf("Initializing periodic event callback\n");

    event_periodic_t periodic_event;
    event_callback_t callback;

    event_periodic_init(&periodic_event);
    event_callback_init(&callback, my_periodic_callback, NULL);

    event_periodic_add(&periodic_event, &callback, 1);

    printf("Periodic event callback started\n");

    while (1) {
        thread_yield();
    }
    return 0;
}