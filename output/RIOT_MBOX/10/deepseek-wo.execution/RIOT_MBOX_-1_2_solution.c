
#include "ztimer.h"
#include "xtimer.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "thread.h"

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define INTERVAL (1 * US_PER_SEC)  // 1 second interval

static char event_sender_stack[STACK_SIZE];
static char event_receiver_stack[STACK_SIZE];

static mbox_t mbox;
static msg_t msg_queue[8];

void *event_sender(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg.content.value = ztimer_now(ZTIMER_USEC);
        mbox_put(&mbox, &msg);
        ztimer_sleep(ZTIMER_USEC, INTERVAL);
    }

    return NULL;
}

void *event_receiver(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg, 1);
        printf("Received event at time: %" PRIu32 "\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    thread_create(event_sender_stack, sizeof(event_sender_stack), PRIORITY,
                  THREAD_CREATE_STACKTEST, event_sender, NULL, "event_sender");

    thread_create(event_receiver_stack, sizeof(event_receiver_stack), PRIORITY,
                  THREAD_CREATE_STACKTEST, event_receiver, NULL, "event_receiver");

    return 0;
}
