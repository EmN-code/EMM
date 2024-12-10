
#include <stdio.h>
#include <thread.h>
#include <sched.h>
#include <event/thread.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

#define STACKSIZE (1024)

void *thread1_func(void *arg)
{
    (void)arg;
    printf("Thread 1 running\n");
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    printf("Thread 2 running\n");
    return NULL;
}

int main(void)
{
    char stack1[STACKSIZE];
    char stack2[STACKSIZE];

    kernel_pid_t pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                      THREAD_CREATE_STACKTEST, thread1_func, NULL, "thread1");

    kernel_pid_t pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                      THREAD_CREATE_STACKTEST, thread2_func, NULL, "thread2");

    // Change priority of thread1 to a higher priority
    sched_change_priority(pid1, THREAD_PRIORITY_MAIN - 3);

    // Change priority of thread2 to a higher priority
    sched_change_priority(pid2, THREAD_PRIORITY_MAIN - 4);

    printf("Main thread running\n");

    return 0;
}
