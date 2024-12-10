
#include "mbox.h"
#include "ztimer.h"
#include "xtimer.h"
#include "msg.h"
#include "timex.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "thread.h"

#define QUEUE_SIZE 8
#define SENDER_STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define RECEIVER_STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

static void *sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 0;

    while (1) {
        msg.content.value++;
        mbox_put(&mbox, &msg);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return NULL;
}

static void *receiver_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %" PRIu32 "\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_create(
        thread_stack, SENDER_STACK_SIZE, THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    thread_create(
        thread_stack, RECEIVER_STACK_SIZE, THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");

    return 0;
}
