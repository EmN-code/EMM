
#include "mbox.h"
#include "ztimer.h"
#include "msg.h"
#include "timex.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"

#define QUEUE_SIZE 8
#define PRIORITY   THREAD_PRIORITY_MAIN
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;

void sender_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1;

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
        mbox_put(&mbox, &msg);
    }
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_create(NULL, STACK_SIZE, PRIORITY, 0, sender_thread, NULL, "sender");

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received message with value: %d\n", msg.content.value);
    }

    return 0;
}
