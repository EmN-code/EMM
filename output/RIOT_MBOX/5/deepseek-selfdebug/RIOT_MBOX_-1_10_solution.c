
#include <stdio.h>
#include "thread.h"
#include "msg.h"
#include "ztimer.h"
#include "event.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void *event_loop(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received event at %" PRIu32 "\n", ztimer_now(ZTIMER_MSEC));
    }

    return NULL;
}

static void periodic_event_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = ztimer_now(ZTIMER_MSEC);
    msg_send(&msg, (kernel_pid_t)arg);
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    kernel_pid_t pid = thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        event_loop,
        NULL,
        "event_loop"
    );

    event_periodic_callback_t periodic_event;
    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &event_loop, periodic_event_callback, (void *)pid);
    event_periodic_callback_set_interval(&periodic_event, 1000);
    event_periodic_callback_start(&periodic_event);

    return 0;
}
