#include <thread_config.h>
#include <kernel_defines.h>

#define THREAD_STACKSIZE_DEFAULT 1024  // Adjust as needed
#define THREAD_PRIORITY_MAIN 5         // Main thread priority

static char stack_one[THREAD_STACKSIZE_DEFAULT];
static char stack_two[THREAD_STACKSIZE_DEFAULT];

static kernel_pid_t thread_one_pid;
static kernel_pid_t thread_two_pid;

void *thread_one(void *arg) {
    (void)arg;
    // Thread one code here
    while (1) {
        // Perform thread tasks
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    // Thread two code here
    while (1) {
        // Perform thread tasks
    }
    return NULL;
}

int main(void) {
    thread_one_pid = thread_create(stack_one, sizeof(stack_one), THREAD_PRIORITY_MAIN + 1,
                                   THREAD_CREATE_SLEEPING | THREAD_CREATE_STACKTEST,
                                   thread_one, NULL, "thread_one");

    thread_two_pid = thread_create(stack_two, sizeof(stack_two), THREAD_PRIORITY_MAIN + 2,
                                   THREAD_CREATE_SLEEPING | THREAD_CREATE_STACKTEST,
                                   thread_two, NULL, "thread_two");

    // Change the priorities
    sched_change_priority(thread_get(thread_one_pid), THREAD_PRIORITY_MAIN + 2);
    sched_change_priority(thread_get(thread_two_pid), THREAD_PRIORITY_MAIN + 1);

    // Resume threads
    thread_wakeup(thread_one_pid);
    thread_wakeup(thread_two_pid);

    return 0;
}