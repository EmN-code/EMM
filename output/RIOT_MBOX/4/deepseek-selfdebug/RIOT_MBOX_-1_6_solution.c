
#include <stdio.h>
#include "thread.h"
#include "msg.h"
#include "ztimer.h"
#include "periph/timer.h" // Include the correct header for timer functionality

#define MSG_QUEUE_SIZE (8)
#define TIMER_INTERVAL (1000000U) // 1 second in microseconds

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

void timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1; // Example message content
    mbox_try_put(&mbox, &msg);
}

void event_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        if (mbox_get(&mbox, &msg, TIMEOUT_NEVER) == 0) {
            printf("Received message with value: %d\n", msg.content.value);
        }
    }
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t pid = thread_create(
        thread_stack, sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        event_thread, NULL, "event_thread");

    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    ztimer_set(ZTIMER_USEC, &timer, TIMER_INTERVAL);

    return 0;
}
