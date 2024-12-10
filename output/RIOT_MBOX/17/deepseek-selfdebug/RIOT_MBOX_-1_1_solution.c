
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define MSG_QUEUE_SIZE     (8)
#define INTERVAL_MS        (1000000U)  /* 1 second interval */

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

static void *event_sender(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        xtimer_usleep(INTERVAL_MS);  /* Wait for the interval */
        msg.content.value = 1;       /* Set some value to the message */
        mbox_put(&mbox, &msg);       /* Send the message to the mailbox */
    }

    return NULL;
}

static void *event_receiver(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);       /* Receive the message from the mailbox */
        printf("Received event with value: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    /* Initialize the mailbox */
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    /* Create threads for sending and receiving events */
    thread_create(
        thread_stack(event_sender),
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        event_sender,
        NULL,
        "event_sender"
    );

    thread_create(
        thread_stack(event_receiver),
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        event_receiver,
        NULL,
        "event_receiver"
    );

    return 0;
}
