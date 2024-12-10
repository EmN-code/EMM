#include <xtimer.h>
#include <stddef.h>
#include <event.h>
#include <msg.h>

void callback(void *arg) {
    (void)arg;  // Unused parameter
    // Code to be executed every second
    printf("Callback executed every second\n");
}

int main(void) {
    static event_periodic_t periodic_event;
    msg_init_queue(NULL, 0);

    event_periodic_init(&periodic_event, 1000000, callback, NULL); // 1 second in microseconds
    event_periodic_start(&periodic_event);

    while (1) {
        event_dispatch(EVENT_QUEUE);  // Dispatch events
    }
    return 0;
}