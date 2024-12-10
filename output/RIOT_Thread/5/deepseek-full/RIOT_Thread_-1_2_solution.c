
#include "msg.h"
#include "thread.h"
#include "xtimer.h"
#include "ztimer.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "evtimer_msg.h"

#define STACKSIZE 1024
#define NUM_MESSAGES 8

static char stack_buffer_high[STACKSIZE];
static char stack_buffer_low[STACKSIZE];

static msg_t msg_queue[NUM_MESSAGES];

static void *high_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message with value: %lu\n", (unsigned long)msg.content.value);
    }

    return NULL;
}

static void *low_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message with value: %lu\n", (unsigned long)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, NUM_MESSAGES);

    kernel_pid_t high_pid = thread_create(stack_buffer_high, sizeof(stack_buffer_high),
                                          THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                          high_priority_thread, NULL, "high_priority");

    kernel_pid_t low_pid = thread_create(stack_buffer_low, sizeof(stack_buffer_low),
                                         THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                                         low_priority_thread, NULL, "low_priority");

    msg_t msg;
    msg.type = 0;

    for (uint32_t i = 0; i < 10; i++) {
        msg.content.value = i;
        if (i % 2 == 0) {
            msg_send(&msg, high_pid);
        } else {
            msg_send(&msg, low_pid);
        }
        xtimer_sleep(1);
    }

    return 0;
}
