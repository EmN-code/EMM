
#include "thread.h"
#include "log.h"  // Include the log header to resolve the implicit declaration errors

#define THREAD_STACKSIZE_DEFAULT (512)

kernel_pid_t thread1_pid, thread2_pid;

void *thread1_func(void *arg)
{
    (void)arg;
    while (1) {
        log_info("Thread 1 is running\n");
        thread_sleep(1000000);  // Sleep for 1 second
    }
    return NULL;
}

void *thread2_func(void *arg)
{
    (void)arg;
    while (1) {
        log_info("Thread 2 is running\n");
        thread_sleep(1000000);  // Sleep for 1 second
    }
    return NULL;
}

int main(void)
{
    log_init();  // Initialize the logging system

    char thread1_stack[THREAD_STACKSIZE_DEFAULT];
    char thread2_stack[THREAD_STACKSIZE_DEFAULT];

    thread1_pid = thread_create(thread1_stack, sizeof(thread1_stack),
                                THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                thread1_func, NULL, "thread1");

    thread2_pid = thread_create(thread2_stack, sizeof(thread2_stack),
                                THREAD_PRIORITY_MAIN - 2, THREAD_CREATE_STACKTEST,
                                thread2_func, NULL, "thread2");

    // Change the priority of thread1 at runtime
    sched_change_priority(thread_get(thread1_pid), THREAD_PRIORITY_MAIN - 3);

    return 0;
}