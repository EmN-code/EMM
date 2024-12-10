#include <thread.h>
#include <stdio.h>

#define STACKSIZE THREAD_STACKSIZE_DEFAULT
#define THREAD_PRIORITY_1 (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_2 (THREAD_PRIORITY_MAIN - 2)

static char thread_stack_1[STACKSIZE];
static char thread_stack_2[STACKSIZE];

void *thread_func_1(void *arg) {
    (void)arg;
    printf("Thread 1 running with priority change.\n");
    return NULL;
}

void *thread_func_2(void *arg) {
    (void)arg;
    printf("Thread 2 running with priority change.\n");
    return NULL;
}

int main(void) {
    kernel_pid_t thread_pid_1 = thread_create(thread_stack_1, STACKSIZE,
                                              THREAD_PRIORITY_1, 0,
                                              thread_func_1, NULL, "thread_1");

    kernel_pid_t thread_pid_2 = thread_create(thread_stack_2, STACKSIZE,
                                              THREAD_PRIORITY_2, 0,
                                              thread_func_2, NULL, "thread_2");
                                              
    printf("Threads created, changing priorities...\n");
    
    sched_change_priority(thread_get(thread_pid_1), THREAD_PRIORITY_2);
    sched_change_priority(thread_get(thread_pid_2), THREAD_PRIORITY_1);

    return 0;
}