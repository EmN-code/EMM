
#include "xtimer.h"
#include "evtimer_mbox.h"
#include "thread.h"
#include "ztimer.h"
#include "msg.h"
#include "event/thread.h"
#include "evtimer_msg.h"

#define STACKSIZE 512
#define MSG_QUEUE_SIZE 8

static char stack_buffer_high[STACKSIZE];
static char stack_buffer_low[STACKSIZE];

static msg_t msg_queue_high[MSG_QUEUE_SIZE];
static msg_queue_t high_priority_queue;

static msg_t msg_queue_low[MSG_QUEUE_SIZE];
static msg_queue_t low_priority_queue;

static void *high_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&high_priority_queue, &msg);
        printf("High priority thread received message: %d\n", msg.content.value);
    }

    return NULL;
}

static void *low_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&low_priority_queue, &msg);
        printf("Low priority thread received message: %d\n", msg.content.value);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue_high, MSG_QUEUE_SIZE);
    msg_init_queue(msg_queue_low, MSG_QUEUE_SIZE);

    kernel_pid_t high_priority_thread_pid = thread_create(stack_buffer_high, sizeof(stack_buffer_high),
                                                          THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                                          high_priority_thread, NULL, "high_priority");

    kernel_pid_t low_priority_thread_pid = thread_create(stack_buffer_low, sizeof(stack_buffer_low),
                                                         THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                                                         low_priority_thread, NULL, "low_priority");

    msg_t msg;
    msg.content.value = 123;

    msg_send(&high_priority_queue, &msg, high_priority_thread_pid, false);
    msg_send(&low_priority_queue, &msg, low_priority_thread_pid, false);

    return 0;
}
