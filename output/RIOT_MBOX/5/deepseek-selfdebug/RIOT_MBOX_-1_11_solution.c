
#include "msg.h"
#include "thread.h"
#include "event.h"
#include "event/periodic_callback.h"
#include "ztimer.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void *event_loop(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Event received!\n");
    }

    return NULL;
}

static void event_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.ptr = NULL;
    msg_send(&msg, thread_getpid());
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    kernel_pid_t event_pid = thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        event_loop,
        NULL,
        "event_loop"
    );

    event_periodic_callback_t periodic_event;
    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &event_thread_queues[EVENT_QUEUE_PRIO_MEDIUM], event_callback, NULL);
    event_periodic_callback_set_interval(&periodic_event, 1000);
    event_periodic_callback_start(&periodic_event);

    return 0;
}
