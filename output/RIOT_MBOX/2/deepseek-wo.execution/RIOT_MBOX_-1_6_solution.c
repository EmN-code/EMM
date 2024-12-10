
#include "evtimer_msg.h"
#include "ztimer.h"
#include "msg.h"
#include "timex.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "thread.h"
#include "xtimer.h"
#include "mbox.h"

#define QUEUE_SIZE 8
#define PRIORITY   THREAD_PRIORITY_MAIN - 1
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)

static char stack[STACK_SIZE];
static msg_t queue[QUEUE_SIZE];
static mbox_t mbox;

static void *event_loop(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received event: %" PRIu32 "\n", (uint32_t)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, queue, QUEUE_SIZE);

    thread_create(stack, sizeof(stack), PRIORITY, THREAD_CREATE_STACKTEST, event_loop, NULL, "event_loop");

    evtimer_init_msg(&mbox);

    msg_t msg;
    msg.content.value = 1;

    while (1) {
        xtimer_sleep(1);
        evtimer_add_msg(&mbox, &msg, timex_set(1, 0));
        msg.content.value++;
    }

    return 0;
}
