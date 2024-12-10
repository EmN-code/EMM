
#include "msg.h"
#include "thread.h"
#include "xtimer.h"
#include "mbox.h"

#define MSG_QUEUE_SIZE     (8)
#define SEND_INTERVAL_MS   (1000000U)  /* 1 second */

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

static void *event_sender(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        /* Send a message to the mailbox */
        msg.content.value = 1;  /* Example content */
        mbox_put(&mbox, &msg);

        /* Wait for the specified interval before sending the next message */
        xtimer_usleep(SEND_INTERVAL_MS);
    }

    return NULL;
}

int main(void)
{
    /* Initialize the message queue */
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    /* Initialize the mailbox */
    mbox_init(&mbox, msg_queue);

    /* Create a thread for sending events */
    thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        event_sender,
        NULL,
        "event_sender"
    );

    /* Main loop to receive messages from the mailbox */
    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %d\n", msg.content.value);
    }

    return 0;
}
