
#include "msg.h"
#include "thread.h"
#include "xtimer.h"
#include "mbox.h"

#define MSG_QUEUE_SIZE     (8)
#define INTERVAL_MS        (1000000U)  /* 1 second interval */

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

static void *event_sender(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        xtimer_usleep(INTERVAL_MS);  /* Wait for 1 second */
        msg.content.value = 1;  /* Set some value to the message */
        mbox_put(&mbox, &msg);  /* Send the message to the mailbox */
    }

    return NULL;
}

static void *event_receiver(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg, 1);  /* Blocking call to get a message from the mailbox */
        printf("Received event with value: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    /* Initialize the message queue */
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    /* Initialize the mailbox */
    mbox_init(&mbox, msg_queue);

    /* Create threads for sending and receiving events */
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, event_sender, NULL, "event_sender");

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, event_receiver, NULL, "event_receiver");

    return 0;
}
