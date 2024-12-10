
#include "xtimer.h"
#include "thread.h"
#include "timex.h"
#include "ztimer.h"
#include "msg.h"
#include "mbox.h"

#define STACKSIZE 512
#define MSG_QUEUE_SIZE 8

kernel_pid_t high_priority_thread_pid;
kernel_pid_t low_priority_thread_pid;

msg_t msg_queue[MSG_QUEUE_SIZE];

void *high_priority_thread(void *arg)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("High priority thread received message: %lu\n", (unsigned long)msg.content.value);
    }
    return NULL;
}

void *low_priority_thread(void *arg)
{
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Low priority thread received message: %lu\n", (unsigned long)msg.content.value);
    }
    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    char high_stack[STACKSIZE];
    char low_stack[STACKSIZE];

    high_priority_thread_pid = thread_create(high_stack, sizeof(high_stack),
                                             THREAD_PRIORITY_MAIN - 1, 0,
                                             high_priority_thread, NULL, "high_priority");

    low_priority_thread_pid = thread_create(low_stack, sizeof(low_stack),
                                            THREAD_PRIORITY_MAIN - 2, 0,
                                            low_priority_thread, NULL, "low_priority");

    msg_t msg;
    msg.type = 0;
    msg.content.value = 123;

    while (1) {
        xtimer_sleep(1);
        msg_send(&msg, high_priority_thread_pid);
        xtimer_sleep(1);
        msg_send(&msg, low_priority_thread_pid);
    }

    return 0;
}
