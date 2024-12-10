#include <stdio.h>
#include <stdint.h>
#include <mutex.h>
#include <msg.h>
#include <kernel.h>

void *thread_one(void *arg) {
    while (1) {
        printf("Thread 1 is running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_two(void *arg) {
    while (1) {
        printf("Thread 2 is running\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    char stack_one[THREAD_STACKSIZE_DEFAULT];
    char stack_two[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t pid_one = thread_create(stack_one, sizeof(stack_one), 
                                         THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, 
                                         thread_one, NULL, "thread_one");
    kernel_pid_t pid_two = thread_create(stack_two, sizeof(stack_two), 
                                         THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, 
                                         thread_two, NULL, "thread_two");

    if (pid_one <= KERNEL_PID_UNDEF || pid_two <= KERNEL_PID_UNDEF) {
        printf("Error creating threads\n");
        return 1;
    }

    printf("Threads started\n");

    while (1) {
        sched_change_priority(thread_get(pid_one), THREAD_PRIORITY_MAIN - 1);
        sched_change_priority(thread_get(pid_two), THREAD_PRIORITY_MAIN - 2);
        printf("Changed priorities\n");
        xtimer_sleep(5);
    }

    return 0;
}