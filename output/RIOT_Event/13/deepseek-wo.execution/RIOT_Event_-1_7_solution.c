
#include "thread.h"
#include "mutex.h"
#include "event/thread.h"
#include "sys/socket.h"
#include "mbox.h"

#define PERIODIC_EVENT_INTERVAL (1000000U)  // 1 second in microseconds

static event_t periodic_event;
static event_queue_t event_queue;

static void periodic_callback(void *arg)
{
    (void)arg;
    // Your periodic task code here
    printf("Periodic event triggered\n");
}

int main(void)
{
    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the periodic event
    periodic_event.handler = periodic_callback;
    periodic_event.context = NULL;

    // Add the periodic event to the event queue
    event_post(&event_queue, &periodic_event);

    // Set up the periodic event to trigger every second
    while (1) {
        xtimer_usleep(PERIODIC_EVENT_INTERVAL);
        event_post(&event_queue, &periodic_event);
    }

    return 0;
}
