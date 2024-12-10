#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define THREAD_PRIORITY_MAIN       (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_SECOND     (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE_MAIN      (THREAD_STACKSIZE_DEFAULT)
#define THREAD_STACKSIZE_SECOND    (THREAD_STACKSIZE_DEFAULT)

char stack_main[THREAD_STACKSIZE_MAIN];
char stack_second[THREAD_STACKSIZE_SECOND];

static kernel_pid_t main_thread_pid;
static kernel_pid_t second_thread_pid;

void *main_thread(void *arg) {
    (void)arg;
    while (1) {
        printf("Main thread is running with priority: %d\n", THREAD_PRIORITY_MAIN);
        // Simulate some work
        xtimer_sleep(2);
        // Change priority of this thread
        printf("Changing priority of main thread to lower priority\n");
        thread_set_priority(main_thread_pid, THREAD_PRIORITY_SECOND);
    }
    return NULL;
}

void *second_thread(void *arg) {
    (void)arg;
    while (1) {
        printf("Second thread is running with priority: %d\n", THREAD_PRIORITY_SECOND);
        // Simulate some work
        xtimer_sleep(2);
        // Change priority of this thread
        printf("Changing priority of second thread to higher priority\n");
        thread_set_priority(second_thread_pid, THREAD_PRIORITY_MAIN);
    }
    return NULL;
}

int main(void) {
    puts("Starting RIOT thread priority example");

    main_thread_pid = thread_create(stack_main, sizeof(stack_main), THREAD_PRIORITY_MAIN,
                                    THREAD_CREATE_STACKTEST, main_thread, NULL, "main_thread");

    second_thread_pid = thread_create(stack_second, sizeof(stack_second), THREAD_PRIORITY_SECOND,
                                      THREAD_CREATE_STACKTEST, second_thread, NULL, "second_thread");

    return 0;
}