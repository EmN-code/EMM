
#include <stdio.h>
#include "thread.h"
#include "msg.h"
#include "event.h"
#include "ztimer.h"

#define MSG_QUEUE_SIZE     (8)
#define EVENT_INTERVAL     (1000000U)  /* 1 second */

static msg_t msg_queue[MSG_QUEUE_SIZE];
static event_queue_t event_queue;

static void event_callback(event_t *event)
{
    msg_t msg;
    msg.content.value = 42;  /* Example value */
    msg_send(&msg, thread_getpid());
}

int main(void)
{
    /* Initialize the message queue */
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    /* Initialize the event queue */
    event_queue_init(&event_queue);

    /* Create a periodic event */
    ztimer_periodic_callback_t periodic_event;
    ztimer_periodic_callback_init(&periodic_event, ZTIMER_USEC, &event_queue, event_callback);
    ztimer_periodic_callback_set_interval(&periodic_event, EVENT_INTERVAL);
    ztimer_periodic_callback_start(&periodic_event);

    /* Main loop */
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with value: %d\n", msg.content.value);
    }

    return 0;
}
