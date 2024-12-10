
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define MSG_QUEUE_SIZE 10
#define HIGH_PRIO_STACK_SIZE 1024
#define LOW_PRIO_STACK_SIZE 1024

kernel_pid_t high_prio_pid;
kernel_pid_t low_prio_pid;

char high_prio_stack[HIGH_PRIO_STACK_SIZE];
char low_prio_stack[LOW_PRIO_STACK_SIZE];

msg_t msg_queue[MSG_QUEUE_SIZE];

void *high_prio_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %d\n", (int)msg.content.value);
    }

    return NULL;
}

void *low_prio_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %d\n", (int)msg.content.value);
    }

    return NULL;
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Create high priority thread
    high_prio_pid = thread_create(high_prio_stack, sizeof(high_prio_stack),
                                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                  high_prio_thread, NULL, "high_prio");

    // Create low priority thread
    low_prio_pid = thread_create(low_prio_stack, sizeof(low_prio_stack),
                                 THREAD_PRIORITY_MAIN + 1, THREAD_CREATE_STACKTEST,
                                 low_prio_thread, NULL, "low_prio");

    // Send messages to threads
    msg_t msg;
    msg.content.value = 1;
    msg_send(&msg, high_prio_pid);

    msg.content.value = 2;
    msg_send(&msg, low_prio_pid);

    return 0;
}
