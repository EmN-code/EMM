
#include <stdio.h>
#include "evtimer_mbox.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "ztimer.h"
#include "msg.h"
#include "mbox.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define QUEUE_SIZE 8
#define PRIORITY  5
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static char stack[STACK_SIZE];

static void *event_sender(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1;

    while (1) {
        xtimer_sleep(1);  // Send event every 1 second
        mbox_put(&mbox, &msg);
    }

    return NULL;
}

static void *event_receiver(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received event with value: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    thread_create(stack, STACK_SIZE, PRIORITY, 0, event_sender, NULL, "event_sender");
    thread_create(stack, STACK_SIZE, PRIORITY, 0, event_receiver, NULL, "event_receiver");

    return 0;
}
