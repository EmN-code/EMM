#include <thread_config.h>
#include <stdio.h>
#include <unistd.h>
#include <thread_arch.h>

#define THREAD_STACKSIZE_MAIN   1024
#define THREAD_PRIORITY_MAIN    5

static char stack_one[THREAD_STACKSIZE_MAIN];
static char stack_two[THREAD_STACKSIZE_MAIN];

void *thread_func_one(void *arg) {
    printf("Thread One starting.\n");
    while (1) {
        printf("Running Thread One.\n");
        sleep(1);
    }
    return NULL;
}

void *thread_func_two(void *arg) {
    printf("Thread Two starting.\n");
    while (1) {
        printf("Running Thread Two.\n");
        sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_one_pid;
    kernel_pid_t thread_two_pid;

    /* Create the first thread */
    thread_one_pid = thread_create(stack_one, sizeof(stack_one),
                                   THREAD_PRIORITY_MAIN, 0,
                                   thread_func_one, NULL, "thread_one");

    /* Create the second thread */
    thread_two_pid = thread_create(stack_two, sizeof(stack_two),
                                   THREAD_PRIORITY_MAIN + 1, 0,
                                   thread_func_two, NULL, "thread_two");

    /* Change the priority of the second thread at runtime */
    sleep(5);
    printf("Changing priority of Thread Two.\n");
    sched_change_priority(thread_two_pid, THREAD_PRIORITY_MAIN - 1);

    while (1) {
        /* Main loop can perform other tasks here */
        printf("Running Main Thread.\n");
        sleep(2);
    }

    return 0;
}