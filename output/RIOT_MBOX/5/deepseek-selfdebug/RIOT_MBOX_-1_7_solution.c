
#include <stdio.h>
#include <xtimer.h>
#include <event/periodic.h>
#include <event/callback.h>
#include <event/thread.h>
#include <mbox.h>

#define INTERVAL_MS (1000U)  // Interval for sending events
#define MSG_QUEUE_SIZE (8)   // Size of the message queue

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

static void event_callback(void *arg)
{
    msg_t msg;
    msg.content.value = (uint32_t)arg;
    mbox_put(&mbox, &msg);
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    // Initialize the event thread
    event_queue_t event_queue;
    event_thread_init(&event_queue, NULL, 0, THREAD_PRIORITY_MAIN - 1);

    // Initialize the periodic event callback
    event_periodic_callback_t periodic_event;
    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &event_queue, event_callback, (void *)1234);

    // Set the interval for the periodic event
    event_periodic_callback_set_interval(&periodic_event, INTERVAL_MS);

    // Start the periodic event
    event_periodic_callback_start(&periodic_event);

    // Main loop to handle received messages
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        printf("Received message with value: %lu\n", (unsigned long)msg.content.value);
    }

    return 0;
}
