#include <thread_config.h>
#include <kernel.h>

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

void *thread_1(void *arg) {
    (void) arg;
    while (1) {
        printf("Thread 1 is running\n");
        xtimer_sleep(2);
    }
    return NULL;
}

void *thread_2(void *arg) {
    (void) arg;
    while (1) {
        printf("Thread 2 is running\n");
        xtimer_sleep(3);
    }
    return NULL;
}

int main(void) {
    printf("Starting thread example\n");

    kernel_pid_t thread_pid1 = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY_MAIN - 1,
                                             THREAD_CREATE_STACKTEST, thread_1, NULL, "thread_1");

    kernel_pid_t thread_pid2 = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY_MAIN - 2,
                                             THREAD_CREATE_STACKTEST, thread_2, NULL, "thread_2");

    // Change priority of threads
    printf("Changing priorities\n");
    thread_set_priority(thread_pid1, THREAD_PRIORITY_MAIN - 3);
    thread_set_priority(thread_pid2, THREAD_PRIORITY_MAIN - 4);

    return 0;
}