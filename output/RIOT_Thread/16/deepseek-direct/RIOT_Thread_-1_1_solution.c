
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
        thread_stack, sizeof(thread_stack),
        HIGH_PRIORITY,
        THREAD_CREATE_STACKTEST,
        high_priority_thread, NULL, "high_priority_thread");

    // Create low priority thread
    thread_create(
        thread_stack, sizeof(thread_stack),
        LOW_PRIORITY,
        THREAD_CREATE_STACKTEST,
        low_priority_thread, NULL, "low_priority_thread");

    // Send messages to threads
    msg_t msg;
    msg.content.ptr = "Message for high priority thread";
    msg_send(&msg, high_priority_thread_pid);

    msg.content.ptr = "Message for low priority thread";
    msg_send(&msg, low_priority_thread_pid);

    return 0;
}
