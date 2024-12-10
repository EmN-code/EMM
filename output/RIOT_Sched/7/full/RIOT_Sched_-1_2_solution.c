#include "thread_config.h"
#include "thread_arch.h"
#include "thread.h"
#include "event/thread.h"
#include "architecture.h"

// Define stack sizes
#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)

// Define the thread priority
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

char thread_stack_one[THREAD_STACKSIZE];
char thread_stack_two[THREAD_STACKSIZE];

static void *thread_one(void *arg) {
    (void)arg;
    // Thread loop
    while(1) {
        // Some thread execution code, e.g., blinking LED
    }
    return NULL;
}

static void *thread_two(void *arg) {
    (void)arg;
    // Thread loop
    while(1) {
        // Some thread execution code, e.g., reading sensor data
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_one_pid, thread_two_pid;

    // Create thread one
    thread_one_pid = thread_create(thread_stack_one, sizeof(thread_stack_one),
                                   THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                                   thread_one, NULL, "thread_one");
    
    // Create thread two
    thread_two_pid = thread_create(thread_stack_two, sizeof(thread_stack_two),
                                   THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
                                   thread_two, NULL, "thread_two");
    
    // Change priorities at runtime
    sched_change_priority(thread_get(thread_one_pid), THREAD_PRIORITY + 1);
    sched_change_priority(thread_get(thread_two_pid), THREAD_PRIORITY - 1);

    // Yield to other threads
    sched_yield();

    return 0;
}