
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
    // Initialize the message box
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    // Create a thread for sending events
    thread_create(
        stack, sizeof(stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        event_sender, NULL, "event_sender");

    // Main loop to receive and process messages
    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received event with value: %d\n", msg.content.value);
    }

    return 0;
}
