
#include "evtimer_msg.h"
#include "ztimer.h"
#include "msg.h"
#include "timex.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "thread.h"
#include "xtimer.h"
#include "mbox.h"

#define STACK_SIZE 512
#define PRIORITY THREAD_PRIORITY_MAIN - 1
#define MSG_QUEUE_SIZE 8

static char stack[STACK_SIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

static void *thread_func(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        if (_mbox_get(&mbox, &msg, 1)) {
            printf("Received message: %d\n", (int)msg.content.value);
        }
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);
    evtimer_init_msg(&evtimer);

    kernel_pid_t pid = thread_create(stack, STACK_SIZE, PRIORITY, THREAD_CREATE_STACKTEST, thread_func, NULL, "thread");

    msg_t msg;
    msg.content.value = 42;

    ztimer_now_t now = ztimer_now(ZTIMER_MSEC);
    ztimer_t timer = { .callback = NULL, .arg = NULL };
    evtimer_add_msg(&evtimer, &timer, &msg, now + ZTIMER_SEC(1));

    return 0;
}
