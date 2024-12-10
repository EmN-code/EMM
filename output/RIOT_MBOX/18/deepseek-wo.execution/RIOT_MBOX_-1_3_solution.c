
#include "mbox.h"
#include "ztimer.h"
#include "xtimer.h"
#include "msg.h"
#include "timex.h"
#include "event/thread.h"
#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "thread.h"

#define QUEUE_SIZE 8
#define PRIORITY  1
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1;

    while (1) {
        xtimer_sleep(5); // Send a message every 5 seconds
        msg_send_receive(&msg, &msg, thread_getpid());
    }
}

void receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_create(
        stack,
        STACK_SIZE,
        PRIORITY,
        THREAD_CREATE_STACKTEST,
        sender_thread,
        NULL,
        "sender"
    );

    thread_create(
        stack,
        STACK_SIZE,
        PRIORITY,
        THREAD_CREATE_STACKTEST,
        receiver_thread,
        NULL,
        "receiver"
    );

    return 0;
}
