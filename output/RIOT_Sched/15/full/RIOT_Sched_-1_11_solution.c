#include <ztimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <msg.h>
#include <thread.h>
#include <sched.h>
#include <kernel_defines.h>

#define THREAD_STACKSIZE_DEFAULT 1024

// Declare global thread variables
static char stack_one[THREAD_STACKSIZE_DEFAULT];
static char stack_two[THREAD_STACKSIZE_DEFAULT];
static kernel_pid_t thread_one_pid;
static kernel_pid_t thread_two_pid;

void *thread_one(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread One running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread Two running\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(NULL, 8);

    thread_one_pid = thread_create(
        stack_one, sizeof(stack_one), THREAD_PRIORITY_DEFAULT - 1,
        THREAD_CREATE_STACKTEST, thread_one, NULL, "thread_one");

    if (thread_one_pid < 0) {
        printf("Failed to start thread one\n");
        return 1;
    }

    thread_two_pid = thread_create(
        stack_two, sizeof(stack_two), THREAD_PRIORITY_DEFAULT - 1,
        THREAD_CREATE_STACKTEST, thread_two, NULL, "thread_two");

    if (thread_two_pid < 0) {
        printf("Failed to start thread two\n");
        return 1;
    }

    // Change the priority of the threads
    sched_change_priority(thread_get(thread_one_pid), THREAD_PRIORITY_DEFAULT - 2);
    sched_change_priority(thread_get(thread_two_pid), THREAD_PRIORITY_DEFAULT - 1);

    return 0;
}