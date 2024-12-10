#include <thread_arch.h>
#include <thread_config.h>
#include <thread.h>

#define THREAD_STACKSIZE_MAIN   (2048)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 10)
#define THREAD_PRIORITY_MEDIUM  (THREAD_PRIORITY_MAIN - 6)

char stack_one[THREAD_STACKSIZE_MAIN];
char stack_two[THREAD_STACKSIZE_MAIN];

void *thread_function(void *arg) {
    (void)arg;
    while (1) {
        // Simulate some work
        thread_yield();
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one, pid_two;
    
    // Initialize threads
    pid_one = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN,
                            CREATE_STACKTEST, thread_function, NULL, "thread_one");

    pid_two = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MEDIUM,
                            CREATE_STACKTEST, thread_function, NULL, "thread_two");
    
    // Simulate a scenario to change thread priority at runtime
    printf("Changing priority of thread_one to medium priority.\n");
    sched_change_priority(thread_get(pid_one), THREAD_PRIORITY_MEDIUM);

    // Yield main thread
    thread_yield();

    return 0;
}