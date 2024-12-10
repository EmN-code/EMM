
#include "thread.h"
#include "mbox.h"
#include "msg.h"
#include "ztimer.h"
#include "event.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE)
static char stack[STACK_SIZE];

static mbox_t mbox;
static msg_t mbox_msg_queue[8];

static void *event_loop(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg, 1);
        printf("Received message: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    mbox_init(&mbox, mbox_msg_queue, 8);

    thread_create(stack, STACK_SIZE, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, event_loop, NULL, "event_loop");

    ztimer_t timer;
    ztimer_set(ZTIMER_MSEC, &timer, 1000);

    while (1) {
        ztimer_wait(&timer);
        msg_t msg;
        msg.content.value = 42;
        mbox_put(&mbox, &msg);
    }

    return 0;
}
