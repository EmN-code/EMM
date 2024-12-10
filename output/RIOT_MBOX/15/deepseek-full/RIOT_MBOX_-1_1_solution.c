
#include <stdio.h>
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define MSG_QUEUE_SIZE 10
#define INTERVAL_MS 1000000  // 1 second in microseconds

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

static void *event_sender(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42;  // Example event value

    while (1) {
        msg_send(&msg, mbox_pid);
        xtimer_usleep(INTERVAL_MS);
    }

    return NULL;
}

int main(void)
{
    // Initialize the message queue
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    // Create a thread for sending events
    thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        event_sender,
        NULL,
        "event_sender"
    );

    msg_t received_msg;

    while (1) {
        // Receive messages from the message box
        mbox_get(&mbox, &received_msg, 1);  // Blocking call
        printf("Received event with value: %d\n", received_msg.content.value);
    }

    return 0;
}
