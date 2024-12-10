#include <thread.h>
#include <event.h>

void callback(void *arg) {
    (void)arg;  // Ignore unused parameter warning
    // Your callback logic here
    printf("Callback triggered\n");
}

int main(void) {
    event_queue_t queue;
    event_periodic_t periodic_event;
    char stack[THREAD_STACKSIZE_MAIN];

    // Initialize event queue
    event_queue_init(&queue);
    
    // Initialize the periodic event, providing the queue, interval and callback
    event_periodic_init(&periodic_event, &queue, SEC_PER_SEC, callback, NULL);

    // Start the periodic event
    event_periodic_start(&periodic_event);

    return 0;
}