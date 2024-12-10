#include <stdio.h>
#include <thread_arch.h>
#include <thread_config.h>

#define THREAD_STACKSIZE         (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN)

static char stack_one[THREAD_STACKSIZE];
static char stack_two[THREAD_STACKSIZE];

static void *thread_one(void *arg) {
    (void)arg;
    printf("Thread One is running\n");
    return NULL;
}

static void *thread_two(void *arg) {
    (void)arg;
    printf("Thread Two is running\n");
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one, pid_two;

    pid_one = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN - 1,
                            THREAD_CREATE_STACKTEST, thread_one, NULL, "thread_one");

    pid_two = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN - 1,
                            THREAD_CREATE_STACKTEST, thread_two, NULL, "thread_two");

    if (pid_one == KERNEL_PID_UNDEF || pid_two == KERNEL_PID_UNDEF) {
        printf("Thread creation failed\n");
        return -1;
    }

    printf("Threads created, changing priorities\n");

    sched_change_priority(pid_one, THREAD_PRIORITY_MAIN - 2);
    sched_change_priority(pid_two, THREAD_PRIORITY_MAIN - 3);

    thread_yield();
    return 0;
}