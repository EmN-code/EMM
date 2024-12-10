#include "esp_wifi.h"
#include "esp_system.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "stdatomic.h"

#define THREAD_PRIORITY_MAIN 5
#define THREAD_STACKSIZE_MAIN (1 << 14)

typedef struct {
    int priority;
    char stack[THREAD_STACKSIZE_MAIN];
} thread_data_t;

void *task_func1(void *arg) {
    printf("Thread 1 running\n");
    return NULL;
}

void *task_func2(void *arg) {
    printf("Thread 2 running\n");
    return NULL;
}

int main(void) {
    thread_data_t thread1 = { .priority = THREAD_PRIORITY_MAIN + 1 };
    thread_data_t thread2 = { .priority = THREAD_PRIORITY_MAIN + 2 };
    
    thread_create(thread1.stack, THREAD_STACKSIZE_MAIN,
                  thread1.priority, 0, task_func1, NULL, "thread1");
    
    thread_create(thread2.stack, THREAD_STACKSIZE_MAIN,
                  thread2.priority, 0, task_func2, NULL, "thread2");
    
    thread_t *t1 = thread_get(thread1.priority);
    thread_t *t2 = thread_get(thread2.priority);
    
    sched_change_priority(t1, thread2.priority);
    sched_change_priority(t2, thread1.priority);
    
    return 0;
}