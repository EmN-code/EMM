
#include "msg.h"
#include "thread.h"
#include "xtimer.h"
#include "mbox.h"
#include "periph/gpio.h"

#define MSG_QUEUE_SIZE     (8)
#define SENDER_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define RECEIVER_PRIO      (THREAD_PRIORITY_MAIN - 2)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

static void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg.content.value = 1; // Example message content
        mbox_put(&mbox, &msg);
        xtimer_sleep(5); // Send a message every 5 seconds
    }

    return NULL;
}

static void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    thread_create(
        thread_stack(sender_stack),
        sizeof(sender_stack),
        SENDER_PRIO,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender"
    );

    thread_create(
        thread_stack(receiver_stack),
        sizeof(receiver_stack),
        RECEIVER_PRIO,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL,
        "receiver"
    );

    return 0;
}
