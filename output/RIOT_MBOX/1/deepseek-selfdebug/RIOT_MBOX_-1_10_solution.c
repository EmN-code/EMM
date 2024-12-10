
#include <stdio.h>
#include "thread.h"
#include "msg.h"
#include "event.h"
#include "ztimer.h"

#define MSG_QUEUE_SIZE (8)
#define EVENT_INTERVAL (1000000U)  /* 1 second in microseconds */

static msg_t msg_queue[MSG_QUEUE_SIZE];
static event_queue_t event_queue;

static void event_handler(void *arg)
{
    (void)arg;
    printf("Event triggered at %lu\n", (unsigned long)ztimer_now(ZTIMER_USEC));
}

int main(void)
{
    /* Initialize the message queue */
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    /* Initialize the event queue */
    event_queue_init(&event_queue);

    /* Create and start a periodic event */
    event_periodic_t periodic_event;
    event_periodic_callback_t periodic_callback;

    event_periodic_callback_init(&periodic_callback, ZTIMER_USEC, &event_queue, event_handler, NULL);
    event_periodic_callback_set_interval(&periodic_callback, EVENT_INTERVAL);
    event_periodic_callback_start(&periodic_callback);

    /* Start the event thread */
    event_thread_init(&event_queue, NULL, 0, THREAD_PRIORITY_MAIN - 1);

    /* Main loop */
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        /* Handle messages if needed */
    }

    return 0;
}
