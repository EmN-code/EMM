
#include "xtimer.h"
#include "evtimer.h"
#include "msg.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static evtimer_t evtimer;
static evtimer_msg_event_t event;

static void periodic_callback(void *arg)
{
    (void)arg;  // Unused parameter
    printf("Periodic callback triggered\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Initialize the event
    event.event.offset = INTERVAL_MS;
    event.msg.content.ptr = NULL;  // No message content needed for this example
    event.msg.type = 0;  // Arbitrary message type

    // Add the event to the event timer
    evtimer_add_msg(&evtimer, &event, thread_getpid());

    // Main loop to keep the application running
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == 0) {
            periodic_callback(NULL);
            evtimer_add_msg(&evtimer, &event, thread_getpid());
        }
    }

    return 0;
}
