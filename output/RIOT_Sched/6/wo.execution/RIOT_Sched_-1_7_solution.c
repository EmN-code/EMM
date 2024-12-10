#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "thread_arch.h"
#include "thread.h"
#include "thread_config.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"

#define STACK_SIZE 1024

char stack1[STACK_SIZE];
char stack2[STACK_SIZE];

void *thread_func(void *arg) {
    (void)arg;
    while(1) {
        // Thread function logic
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1, 0, thread_func, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2, 0, thread_func, NULL, "thread2");

    // Modify priority at runtime
    uint8_t new_priority = THREAD_PRIORITY_MAIN - 3; // Adjust priority as needed

    thread_set_priority(thread1, new_priority);
    thread_set_priority(thread2, new_priority);

    while (1) {
        // Main thread can perform tasks or sleep
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}