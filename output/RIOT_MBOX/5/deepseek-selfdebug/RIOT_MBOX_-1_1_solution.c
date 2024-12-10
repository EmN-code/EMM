
#include "msg.h"
#include "thread.h"
#include "xtimer.h"
#include "mbox.h"

#define MSG_QUEUE_SIZE    10
#define INTERVAL_MS       1000000  // 1 second interval

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

static void *event_sender(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        xtimer_usleep(INTERVAL_MS);  // Wait for the interval
        msg.content.value = 1;  // Set some value to the message
        mbox_put(&mbox, &msg);  // Send the message to the mailbox
    }

    return NULL;
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    // Create a thread for sending events
    thread_create(
        thread_stack, sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        event_sender, NULL, "event_sender"
    );

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);  // Wait for a message from the mailbox
        printf("Received event with value: %d\n", msg.content.value);
    }

    return 0;
}
