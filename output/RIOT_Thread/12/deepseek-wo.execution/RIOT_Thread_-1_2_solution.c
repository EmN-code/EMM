
#include <stdio.h>
#include <inttypes.h>
#include "thread.h"
#include "msg.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "event/thread.h"
#include "xtimer.h"

#define STACKSIZE 512
#define NUM_MESSAGES 8

static char stack_buffer_high[STACKSIZE];
static char stack_buffer_low[STACKSIZE];

static msg_t msg_queue[NUM_MESSAGES];

static void *high_priority_thread(void *arg)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message with value: %" PRIu32 "\n", msg.content.value);
    }
    return NULL;
}

static void *low_priority_thread(void *arg)
{
    msg_t msg;
    msg.content.value = 42;
    msg.type = 1;

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
        msg_send(&msg, (kernel_pid_t)arg);
    }
    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, NUM_MESSAGES);

    kernel_pid_t high_pid = thread_create(stack_buffer_high,
                                          STACKSIZE,
                                          THREAD_PRIORITY_MAIN - 1,
                                          THREAD_CREATE_STACKTEST,
                                          high_priority_thread,
                                          NULL,
                                          "high_priority");

    kernel_pid_t low_pid = thread_create(stack_buffer_low,
                                         STACKSIZE,
                                         THREAD_PRIORITY_MAIN - 2,
                                         THREAD_CREATE_STACKTEST,
                                         low_priority_thread,
                                         (void *)high_pid,
                                         "low_priority");

    (void)low_pid; // To suppress unused variable warning

    return 0;
}
