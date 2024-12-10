
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
        xtimer_sleep(SEND_INTERVAL_MS);
        msg.content.value = 1;  /* Example event value */
        mbox_put(&mbox, &msg);
    }

    return NULL;
}

static void *event_receiver(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg, 1);  /* Blocking call */
        printf("Received event with value: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    /* Initialize the message queue */
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    /* Create threads for sending and receiving events */
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, event_sender, NULL, "event_sender");

    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, event_receiver, NULL, "event_receiver");

    return 0;
}
