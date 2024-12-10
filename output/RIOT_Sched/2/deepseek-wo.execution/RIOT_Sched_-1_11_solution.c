
#include "thread.h"
#include "sched.h"
#include "msg.h"
#include "assert.h"

#define THREAD_STACKSIZE_DEFAULT (512)

kernel_pid_t thread1_pid;
kernel_pid_t thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Thread 1 received message\n");
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Thread 2 received message\n");
    }
    return NULL;
}

int main(void)
{
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    // Change priority of thread1 to a higher priority
    sched_change_priority(thread_get_from_pid(thread1_pid), THREAD_PRIORITY_MAIN - 3);

    // Change priority of thread2 to a lower priority
    sched_change_priority(thread_get_from_pid(thread2_pid), THREAD_PRIORITY_MAIN - 4);

    msg_t msg;
    msg.content.value = 0;

    // Send a message to thread1
    msg_send(&msg, thread1_pid);

    // Send a message to thread2
    msg_send(&msg, thread2_pid);

    return 0;
}
