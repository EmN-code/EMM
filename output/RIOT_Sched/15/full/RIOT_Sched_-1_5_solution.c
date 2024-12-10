#include "ztimer.h"
#include "timex.h"
#include "thread_arch.h"
#include "thread_config.h"
#include "thread.h"
#include "sys/socket.h"
#include "stdio.h"
#include "stdlib.h"
#include "sched.h"

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)

static char thread_one_stack[THREAD_STACKSIZE_MAIN];
static char thread_two_stack[THREAD_STACKSIZE_MAIN];

void* thread_one(void *arg) {
    (void)arg;
    printf("Thread one is running with priority %u\n", thread_get_priority(thread_getpid()));
    return NULL;
}

void* thread_two(void *arg) {
    (void)arg;
    printf("Thread two is running with priority %u\n", thread_get_priority(thread_getpid()));
    return NULL;
}

int main(void) {
    printf("Starting two threads\n");

    kernel_pid_t pid_one = thread_create(thread_one_stack, sizeof(thread_one_stack),
                                         THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                                         thread_one, NULL, "thread_one");

    kernel_pid_t pid_two = thread_create(thread_two_stack, sizeof(thread_two_stack),
                                         THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                         thread_two, NULL, "thread_two");

    printf("Created two threads with PIDs %d and %d\n", pid_one, pid_two);

    ztimer_sleep(ZTIMER_MSEC, 1000);

    printf("Changing priority of thread one\n");
    sched_change_priority(thread_get(pid_one), THREAD_PRIORITY_MAIN - 2);

    return 0;
}