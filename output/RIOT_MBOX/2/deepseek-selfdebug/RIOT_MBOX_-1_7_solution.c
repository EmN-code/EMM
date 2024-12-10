
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include <event/callback.h>
#include <event/periodic.h>
#include <mbox.h>

#define MSG_QUEUE_SIZE 16
#define INTERVAL_MS 1000000

static mbox_t mbox;
static msg_t msg_queue[MSG_QUEUE_SIZE];

static void event_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Example value
    mbox_put(&mbox, &msg);
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    // Initialize the periodic event callback
    event_periodic_callback_t periodic_event;
    event_periodic_callback_init(&periodic_event, ZTIMER_USEC, &mbox, event_callback, NULL);
    event_periodic_callback_set_interval(&periodic_event, INTERVAL_MS);
    event_periodic_callback_start(&periodic_event);

    // Main loop to handle messages from the mailbox
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        printf("Received message with value: %d\n", (int)msg.content.value);
    }

    return 0;
}
