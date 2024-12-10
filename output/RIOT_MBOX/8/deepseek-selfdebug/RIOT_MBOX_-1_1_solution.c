
#include <stdio.h>
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define MSG_QUEUE_SIZE 10
#define INTERVAL_MS 1000000  // 1 second interval

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
    thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, 0, event_sender, NULL, "event_sender");

    // Main loop to receive and process events
    msg_t received_msg;
    while (1) {
        mbox_get(&mbox, &received_msg, 1);  // Blocking call to receive messages
        printf("Received event with value: %d\n", received_msg.content.value);
    }

    return 0;
}
