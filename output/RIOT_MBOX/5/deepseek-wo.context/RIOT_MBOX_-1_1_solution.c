
#include "msg.h"
#include "thread.h"
#include "xtimer.h"
#include "mbox.h"

#define MSG_QUEUE_SIZE     (8)
#define SEND_INTERVAL_MS   (1000000U)  /* 1 second */

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

static void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        /* Send a message to the receiver thread */
        msg.content.value = 1; /* Example content */
        mbox_put(&mbox, &msg);

        /* Wait for 1 second before sending the next message */
        xtimer_sleep(SEND_INTERVAL_MS / 1000000U);
    }

    return NULL;
}

static void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        /* Receive a message from the message box */
        mbox_get(&mbox, &msg);

        /* Process the received message */
        printf("Received message with value: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    /* Initialize the message box */
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    /* Create the sender thread */
    thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender"
    );

    /* Create the receiver thread */
    thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL,
        "receiver"
    );

    return 0;
}
