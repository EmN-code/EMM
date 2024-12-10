
#include <stdio.h>
#include <stdlib.h>
#include "thread.h"
#include "msg.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "event/thread.h"
#include "xtimer.h"

#define STACKSIZE 512
#define NUM_MESSAGES 8

static char stack_low[STACKSIZE];
static char stack_high[STACKSIZE];

static msg_t msg_queue[NUM_MESSAGES];

void *low_priority_thread(void *arg)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %lu\n", (unsigned long)msg.content.value);
    }
    return NULL;
}

void *high_priority_thread(void *arg)
{
    msg_t msg;
    msg.content.value = 1234;
    msg.type = 0;

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
        msg_send(&msg, thread_getpid());
        printf("High priority thread sent message: %lu\n", (unsigned long)msg.content.value);
    }
    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, NUM_MESSAGES);

    kernel_pid_t pid_low = thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_MAIN - 1,
                                         THREAD_CREATE_STACKTEST, low_priority_thread, NULL, "low");

    kernel_pid_t pid_high = thread_create(stack_high, sizeof(stack_high), THREAD_PRIORITY_MAIN + 1,
                                          THREAD_CREATE_STACKTEST, high_priority_thread, NULL, "high");

    msg_t msg;
    msg.content.ptr = (void *)pid_low;
    msg.type = 0;

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 2000);
        msg_send(&msg, pid_high);
        printf("Main thread sent message to high priority thread\n");
    }

    return 0;
}
