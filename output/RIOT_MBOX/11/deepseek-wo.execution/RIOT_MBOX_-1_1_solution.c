
#include "msg.h"
#include "thread.h"
#include "xtimer.h"
#include "mbox.h"

#define MSG_QUEUE_SIZE    10
#define SEND_INTERVAL     1000000  // 1 second in microseconds

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

static void *event_sender(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        // Send a message to the mailbox
        msg.content.value = 1;  // Example message content
        mbox_put(&mbox, &msg);

        // Wait for the specified interval
        xtimer_usleep(SEND_INTERVAL);
    }

    return NULL;
}

static void *event_receiver(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        // Receive a message from the mailbox
        mbox_get(&mbox, &msg);

        // Process the received message
        printf("Received message with value: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    // Create threads for sending and receiving events
    thread_create(
        stack, sizeof(stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        event_sender, NULL, "event_sender");

    thread_create(
        stack, sizeof(stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        event_receiver, NULL, "event_receiver");

    return 0;
}
