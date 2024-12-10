
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <sched.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <kernel_defines.h>

#define STACK_SIZE 512
#define THREAD_PRIORITY_HIGH 5
#define THREAD_PRIORITY_LOW 10

static char stack1[STACK_SIZE];
static char stack2[STACK_SIZE];

void *thread_func1(void *arg)
{
    (void)arg;
    printf("Thread 1 running at priority %d\n", thread_get_priority(thread_get_active()));
    return NULL;
}

void *thread_func2(void *arg)
{
    (void)arg;
    printf("Thread 2 running at priority %d\n", thread_get_priority(thread_get_active()));
    return NULL;
}

int main(void)
{
    kernel_pid_t pid1, pid2;

    // Create the first thread
    pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_HIGH, 0, thread_func1, NULL, "thread1");
    if (pid1 < 0) {
        printf("Failed to create thread 1\n");
        return 1;
    }

    // Create the second thread
    pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_LOW, 0, thread_func2, NULL, "thread2");
    if (pid2 < 0) {
        printf("Failed to create thread 2\n");
        return 1;
    }

    // Change the priority of the second thread at runtime
    sched_change_priority(thread_get(pid2), THREAD_PRIORITY_HIGH);

    // Print the new priority of the second thread
    printf("Thread 2 priority changed to %d\n", thread_get_priority(thread_get(pid2)));

    return 0;
}
