
#include <stdio.h>
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/timer.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define TIMER_INTERVAL      (1000000U)  /* 1 second */
#define TIMER_DEV           TIMER_DEV(0)

static void *timer_thread(void *arg)
{
    (void)arg;
    msg_t msg;
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    while (1) {
        msg_receive(&msg);
        printf("Event received!\n");
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    kernel_pid_t timer_pid = thread_create(
        thread_stack(timer_thread),
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        timer_thread,
        NULL,
        "timer_thread"
    );

    timer_t timer;
    timer_init(TIMER_DEV, 1000000, NULL, NULL);
    timer_set_periodic(TIMER_DEV, 0, TIMER_INTERVAL, 0);

    while (1) {
        xtimer_sleep(1);
        msg_t msg;
        msg.content.value = 0;
        msg_send(&msg, timer_pid);
    }

    return 0;
}
