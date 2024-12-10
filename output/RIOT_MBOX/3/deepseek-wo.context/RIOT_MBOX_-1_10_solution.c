
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include <event/callback.h>
#include <event/periodic_callback.h>
#include <mbox.h>

#define INTERVAL_MS (1000 * 1000U)  // 1 second interval
#define MSG_QUEUE_SIZE (8)

static mbox_t mbox;
static msg_t msg_queue[MSG_QUEUE_SIZE];

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

    // Create and start the periodic event
    event_periodic_callback_t event;
    event_periodic_callback_create(&event, ZTIMER_USEC, INTERVAL_MS, &mbox, event_callback, (void *)1234);
    event_periodic_callback_start(&event, INTERVAL_MS);

    // Main loop to handle messages from the mailbox
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return 0;
}
