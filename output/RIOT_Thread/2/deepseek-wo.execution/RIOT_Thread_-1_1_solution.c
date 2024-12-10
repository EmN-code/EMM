
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define MSG_QUEUE_SIZE 10
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

kernel_pid_t high_priority_thread_pid;
kernel_pid_t low_priority_thread_pid;

void *high_priority_thread(void *arg)
{
    msg_t msg_queue[MSG_QUEUE_SIZE];
    msg_t msg;

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %s\n", (char *)msg.content.ptr);
        xtimer_sleep(1);
    }

    return NULL;
}

void *low_priority_thread(void *arg)
{
    msg_t msg_queue[MSG_QUEUE_SIZE];
    msg_t msg;

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %s\n", (char *)msg.content.ptr);
        xtimer_sleep(1);
    }

    return NULL;
}

int main(void)
{
    msg_t msg;

    // Initialize threads
    thread_create(
        stack_high, sizeof(stack_high),
        PRIORITY_HIGH,
        THREAD_CREATE_STACKTEST,
        high_priority_thread, NULL, "high_priority_thread");

    thread_create(
        stack_low, sizeof(stack_low),
        PRIORITY_LOW,
        THREAD_CREATE_STACKTEST,
        low_priority_thread, NULL, "low_priority_thread");

    // Send messages to threads
    msg.type = 0;
    msg.content.ptr = "Message to high priority thread";
    msg_send(&msg, high_priority_thread_pid);

    msg.content.ptr = "Message to low priority thread";
    msg_send(&msg, low_priority_thread_pid);

    return 0;
}
