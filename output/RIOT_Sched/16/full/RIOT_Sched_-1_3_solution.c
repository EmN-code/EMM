#include "thread_arch.h"
#include "thread_config.h"
#include "thread.h"
#include "architecture.h"
#include "MQTTClient.h"

#include <stdio.h>
#include <stdint.h>

void *thread_func1(void *arg) {
    printf("Thread 1 is running\n");
    return NULL;
}

void *thread_func2(void *arg) {
    printf("Thread 2 is running\n");
    return NULL;
}

int main(void) {
    char stack1[THREAD_STACKSIZE_DEFAULT];
    char stack2[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t thread1 = thread_create(stack1, sizeof(stack1), 10, CREATE_STACKTEST, thread_func1, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2), 11, CREATE_STACKTEST, thread_func2, NULL, "thread2");

    // Let each thread run once
    thread_yield();
    thread_yield();

    // Change priorities
    thread_set_priority(thread1, 11);
    thread_set_priority(thread2, 10);

    // Let each thread run again with new priorities
    thread_yield();
    thread_yield();

    return 0;
}