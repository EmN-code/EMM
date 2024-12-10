
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define MSG_QUEUE_SIZE    10
#define INTERVAL_MS       1000

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

static void *event_sender(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg.content.value = 1; // Example event value
        msg_send(&msg, mbox_pid);
        xtimer_sleep(INTERVAL_MS / MS_IN_USEC);
    }

    return NULL;
}

static void *event_receiver(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received event with value: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    thread_create(
        thread_stack(event_sender_stack),
        sizeof(event_sender_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        event_sender,
        NULL,
        "event_sender"
    );

    thread_create(
        thread_stack(event_receiver_stack),
        sizeof(event_receiver_stack),
        THREAD_PRIORITY_MAIN - 2,
        THREAD_CREATE_STACKTEST,
        event_receiver,
        NULL,
        "event_receiver"
    );

    return 0;
}
