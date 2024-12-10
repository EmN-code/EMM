#include <xtimer.h>  // Included for timing operations
#include <thread.h>  // Core include for managing threads

// Define stack size to avoid redefinition conflict
#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT)

// Define thread priority uniquely to avoid redefinition conflict
#define THREAD_PRIORITY_MAIN  (7)

char stack_one[THREAD_STACKSIZE_MAIN];
char stack_two[THREAD_STACKSIZE_MAIN];

void *thread_one_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread One is running.\n");
        xtimer_sleep(1);
    }
    return NULL;
}

void *thread_two_func(void *arg) {
    (void)arg;
    while (1) {
        printf("Thread Two is running.\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_one_pid;
    kernel_pid_t thread_two_pid;

    thread_one_pid = thread_create(stack_one, sizeof(stack_one),
                                   THREAD_PRIORITY_MAIN - 1, 0,
                                   thread_one_func, NULL, "thread_one");

    thread_two_pid = thread_create(stack_two, sizeof(stack_two),
                                   THREAD_PRIORITY_MAIN - 2, 0,
                                   thread_two_func, NULL, "thread_two");
    
    // Change the priority of thread one
    thread_t *thread_one = thread_get(thread_one_pid); 
    if (thread_one != NULL) {
        sched_change_priority(thread_one, THREAD_PRIORITY_MAIN - 3);
    }

    // Change the priority of thread two
    thread_t *thread_two = thread_get(thread_two_pid);
    if (thread_two != NULL) {
        sched_change_priority(thread_two, THREAD_PRIORITY_MAIN - 4);
    }

    return 0;
}