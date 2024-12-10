#include <ztimer.h>
#include <stdio.h>
#include <msg.h>
#include <evtimer.h>

#define MSG_TYPE_EVENT (0xAB)
#define EVENT_INTERVAL (1U * ZTIMER_MSEC) // 1 second

static evtimer_t evtimer;
static msg_t msg_queue[8];

static void event_callback(evtimer_event_t *event)
{
    (void)event; // Unused parameter
    
    msg_t msg;
    msg.type = MSG_TYPE_EVENT;
    
    // Send the event message to the thread's own message queue
    msg_send(&msg, thread_getpid());

    printf("Event triggered and message sent!\n");
}

int main(void)
{
    msg_init_queue(msg_queue, 8);
    printf("Message queue initialized.\n");

    evtimer_event_t event;
    evtimer_set(&event, event_callback, NULL, EVENT_INTERVAL);

    evtimer_init(&evtimer, ZTIMER_MSEC);
    evtimer_add(&evtimer, &event);

    printf("Event timer started with interval: %u milliseconds.\n", (unsigned)EVENT_INTERVAL);

    while (1) {
        msg_t msg;
        // Blocking wait for incoming messages
        msg_receive(&msg);

        if (msg.type == MSG_TYPE_EVENT) {
            printf("Event message received!\n");
            // Handle the event message here, if necessary
        }
    }

    return 0;
}