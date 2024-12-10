#include <thread_arch.h>
#include <thread_config.h>

static char stack_one[THREAD_STACKSIZE_MAIN];
static char stack_two[THREAD_STACKSIZE_MAIN];

void *thread_one(void *arg) {
    while (1) {
        printf("Thread One is running\n");
        thread_yield();
    }
    return NULL;
}

void *thread_two(void *arg) {
    while (1) {
        printf("Thread Two is running\n");
        thread_yield();
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid1, pid2;
    
    pid1 = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN + 1,
                         CREATE_STACKTEST, thread_one, NULL, "thread_one");
    printf("Created Thread One with PID %d\n", pid1);

    pid2 = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN + 2,
                         CREATE_STACKTEST, thread_two, NULL, "thread_two");
    printf("Created Thread Two with PID %d\n", pid2);

    // Simulate runtime priority change
    xtimer_sleep(5);
    printf("Changing priority of Thread One\n");
    sched_change_priority(pid1, THREAD_PRIORITY_MAIN + 3);

    return 0;
}