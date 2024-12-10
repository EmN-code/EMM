#include <thread_arch.h>
#include <kernel_defines.h>
#include <stdio.h>
#include <thread.h>

void *thread_handler(void *arg) {
    (void)arg;
    printf("Thread is running\n");
    return NULL;
}

int main(void) {
    puts("RIOT Thread Management Example\n");

    char stack1[THREAD_STACKSIZE_MAIN];
    char stack2[THREAD_STACKSIZE_MAIN];

    kernel_pid_t thread1_pid;
    kernel_pid_t thread2_pid;

    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                THREAD_CREATE_SLEEPING, thread_handler, NULL, "thread 1");
    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                THREAD_CREATE_SLEEPING, thread_handler, NULL, "thread 2");

    thread_wakeup(thread1_pid);
    thread_wakeup(thread2_pid);

    printf("Changing priorities at runtime\n");
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 3);
    sched_change_priority(thread_get(thread2_pid), THREAD_PRIORITY_MAIN - 4);

    return 0;
}