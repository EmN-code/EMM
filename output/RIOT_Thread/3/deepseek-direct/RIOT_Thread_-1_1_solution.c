
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define MSG_QUEUE_SIZE 10
#define HIGH_PRIORITY 7
#define LOW_PRIORITY 5

static msg_t msg_queue[MSG_QUEUE_SIZE];
static kernel_pid_t high_priority_thread_pid;
static kernel_pid_t low_priority_thread_pid;

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

    // Create high priority thread
    thread_create(
        stack, sizeof(stack),
        HIGH_PRIORITY,
        THREAD_CREATE_STACKTEST,
        high_priority_thread,
        NULL, "high_priority_thread");

    // Create low priority thread
    thread_create(
        stack, sizeof(stack),
        LOW_PRIORITY,
        THREAD_CREATE_STACKTEST,
        low_priority_thread,
        NULL, "low_priority_thread");

    msg_t msg;
    char *high_msg = "Message for high priority thread";
    char *low_msg = "Message for low priority thread";

    while (1) {
        // Send message to high priority thread
        msg.content.ptr = high_msg;
        msg_send(&msg, high_priority_thread_pid);

        xtimer_sleep(1);

        // Send message to low priority thread
        msg.content.ptr = low_msg;
        msg_send(&msg, low_priority_thread_pid);

        xtimer_sleep(1);
    }

    return 0;
}
