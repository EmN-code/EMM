#include "esp_wifi.h"
#include "esp_system.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "sched.h"
#include "thread.h"
#include "mutex.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN (10U)

char stack1[THREAD_STACKSIZE];
char stack2[THREAD_STACKSIZE];

void *thread_func1(void *arg) {
    (void)arg;
    printf("Thread 1 is running\n");
    return NULL;
}

void *thread_func2(void *arg) {
    (void)arg;
    printf("Thread 2 is running\n");
    return NULL;
}

int main(void) {
    kernel_pid_t thread1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN + 1, 0, thread_func1, NULL, "thread1");
    kernel_pid_t thread2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN + 2, 0, thread_func2, NULL, "thread2");
    
    if (thread1 >= 0 && thread2 >= 0) {
        printf("Threads created successfully.\n");
    }
    
    printf("Changing priority of thread2.\n");
    sched_change_priority(thread_get(thread2), THREAD_PRIORITY_MAIN + 3);

    return 0;
}