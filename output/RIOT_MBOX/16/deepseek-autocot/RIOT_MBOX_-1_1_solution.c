
#include "msg.h"
#include "thread.h"
#include "xtimer.h"
#include "mbox.h"

#define MSG_QUEUE_SIZE     (8)
#define SENDER_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO      (THREAD_PRIORITY_MAIN - 2)
#define INTERVAL_MS        (1000000U)  /* 1 second interval */

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

static void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg.content.value = 1;  /* Example message content */
        mbox_put(&mbox, &msg);
        xtimer_usleep(INTERVAL_MS);
    }

    return NULL;
}

static void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    /* Initialize the message box */
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    /* Create sender thread */
    thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        SENDER_PRIO,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender"
    );

    /* Create receiver thread */
    thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        RECEIVER_PRIO,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL,
        "receiver"
    );

    return 0;
}
