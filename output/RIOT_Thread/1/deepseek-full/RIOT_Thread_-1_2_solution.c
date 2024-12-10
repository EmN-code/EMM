
#include "thread.h"
#include "xtimer.h"
#include "ztimer.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "evtimer_msg.h"

#define STACKSIZE 512
#define NUM_MESSAGES 8

static char stack_buffer_high[STACKSIZE];
static char stack_buffer_low[STACKSIZE];

static msg_t msg_array[NUM_MESSAGES];
static kernel_pid_t high_priority_pid;
static kernel_pid_t low_priority_pid;

void *high_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", msg.content.value);
    }

    return NULL;
}

void *low_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_array, NUM_MESSAGES);

    high_priority_pid = thread_create(stack_buffer_high, STACKSIZE, THREAD_PRIORITY_MAIN - 1,
                                      THREAD_CREATE_STACKTEST, high_priority_thread, NULL, "high_priority");

    low_priority_pid = thread_create(stack_buffer_low, STACKSIZE, THREAD_PRIORITY_MAIN - 2,
                                     THREAD_CREATE_STACKTEST, low_priority_thread, NULL, "low_priority");

    msg_t msg;
    msg.content.value = 123;

    // Send a message to the high priority thread
    msg_send(&msg, high_priority_pid);

    // Send a message to the low priority thread
    msg_send(&msg, low_priority_pid);

    return 0;
}
