#include "thread_arch.h"
#include "event/thread.h"
#include "architecture.h"
#include "thread_config.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include <stdio.h>

#define STACKSIZE_THREAD 1024

static char stack1[STACKSIZE_THREAD];
static char stack2[STACKSIZE_THREAD];

void *thread1(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 1 running with priority: %u\n", thread_get_priority(thread_getpid()));
        thread_yield();
    }
    return NULL;
}

void *thread2(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread 2 running with priority: %u\n", thread_get_priority(thread_getpid()));
        thread_yield();
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid, thread2_pid;

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread1, NULL, "thread1");
    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread2, NULL, "thread2");

    printf("Main thread running\n");
    thread_set_priority(thread1_pid, THREAD_PRIORITY_MAIN - 1);
    thread_set_priority(thread2_pid, THREAD_PRIORITY_MAIN - 2);

    while (1) {
        /* Change priority at runtime */
        thread_set_priority(thread1_pid, THREAD_PRIORITY_MAIN - 2);
        thread_set_priority(thread2_pid, THREAD_PRIORITY_MAIN - 1);
        thread_yield();
        thread_set_priority(thread1_pid, THREAD_PRIORITY_MAIN - 1);
        thread_set_priority(thread2_pid, THREAD_PRIORITY_MAIN - 2);
        thread_yield();
    }

    return 0;
}