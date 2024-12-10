
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread.h>
#include <sched.h>
#include <kernel_defines.h>

#define STACKSIZE 1024
#define PRIORITY_HIGH 5
#define PRIORITY_LOW 10

char stack_high[STACKSIZE];
char stack_low[STACKSIZE];

void *high_priority_thread(void *arg) {
    printf("High priority thread started\n");
    while (1) {
        printf("High priority thread running\n");
        sleep(2);
    }
    return NULL;
}

void *low_priority_thread(void *arg) {
    printf("Low priority thread started\n");
    while (1) {
        printf("Low priority thread running\n");
        sleep(2);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_high, pid_low;

    pid_high = thread_create(stack_high, sizeof(stack_high), PRIORITY_HIGH, 0, high_priority_thread, NULL, "high_priority");
    pid_low = thread_create(stack_low, sizeof(stack_low), PRIORITY_LOW, 0, low_priority_thread, NULL, "low_priority");

    if (pid_high <= KERNEL_PID_UNDEF || pid_low <= KERNEL_PID_UNDEF) {
        printf("Failed to create threads\n");
        return 1;
    }

    printf("Threads created successfully\n");

    sleep(5);

    printf("Changing priority of high priority thread to low\n");
    sched_change_priority(pid_high, PRIORITY_LOW);

    sleep(5);

    printf("Changing priority of low priority thread to high\n");
    sched_change_priority(pid_low, PRIORITY_HIGH);

    return 0;
}
