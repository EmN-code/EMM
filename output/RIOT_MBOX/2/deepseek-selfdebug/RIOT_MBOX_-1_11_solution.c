
#include <stdio.h>
#include <thread.h>
#include <mbox.h>
#include <event/periodic.h>
#include <event/callback.h>

#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)

static mbox_t mbox;
static msg_t msg_array[8];
static char stack[STACK_SIZE];

void periodic_event_callback(void *arg)
{
    msg_t msg;
    msg.content.value = (uint32_t)arg;
    mbox_put(&mbox, &msg);
}

void event_handler(void)
{
    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg, -1);
        printf("Received event with value: %" PRIu32 "\n", msg.content.value);
    }
}

int main(void)
{
    mbox_init(&mbox, msg_array, sizeof(msg_array) / sizeof(msg_array[0]));

    event_periodic_callback_t periodic_event;
    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, 1000, &mbox, periodic_event_callback);
    event_periodic_callback_start(&periodic_event);

    thread_create(stack, sizeof(stack), PRIORITY, THREAD_CREATE_STACKTEST, (thread_task_func_t)event_handler, NULL, "event_handler");

    return 0;
}
