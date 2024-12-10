
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define MSG_QUEUE_SIZE 10
#define HIGH_PRIORITY 7
#define LOW_PRIORITY 3

static char stack_high[THREAD_STACKSIZE_DEFAULT];
static char stack_low[THREAD_STACKSIZE_DEFAULT];

static msg_t msg_queue[MSG_QUEUE_SIZE];

void *high_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %s\n", (char *)msg.content.ptr);
    }

    return NULL;
}

void *low_priority_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %s\n", (char *)msg.content.ptr);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    thread_create(stack_high, sizeof(stack_high), HIGH_PRIORITY, THREAD_CREATE_STACKTEST, high_priority_thread, NULL, "high_priority");
    thread_create(stack_low, sizeof(stack_low), LOW_PRIORITY, THREAD_CREATE_STACKTEST, low_priority_thread, NULL, "low_priority");

    msg_t msg;
    msg.type = 0;

    while (1) {
        msg.content.ptr = "Message for high priority thread";
        msg_send(&msg, thread_getpid(high_priority_thread));

        xtimer_sleep(1);

        msg.content.ptr = "Message for low priority thread";
        msg_send(&msg, thread_getpid(low_priority_thread));

        xtimer_sleep(1);
    }

    return 0;
}
