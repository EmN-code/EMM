#include "unistd.h"  // Required for sleep function
#include "time_units.h" // For time unit conversion macros
#include "thread_arch.h"
#include "thread_config.h"  // For thread priority and stack size
#include "thread.h"
#include "stdatomic.h"  // Supports atomic operations
#include "stdbool.h"  // For boolean data type
#include "sched.h"
#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"
#include "MQTTClient.h"
#include "kernel_defines.h"
#include "architecture.h"
#include "event/thread.h"

#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack1[THREAD_STACKSIZE];
static char stack2[THREAD_STACKSIZE];

void *thread_handler(void *arg) {
    while (1) {
        printf("Thread %s is running with priority %d\n", (char *)arg, THREAD_PRIORITY + 1);
        sleep(1);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1_pid, thread2_pid;

    // Create first thread
    thread1_pid = thread_create(stack1, sizeof(stack1), THREAD_PRIORITY, 0, thread_handler, "1", "thread1");
    if (thread1_pid <= KERNEL_PID_UNDEF) {
        printf("Error: Unable to create thread 1\n");
        return 1;
    }

    // Create second thread
    thread2_pid = thread_create(stack2, sizeof(stack2), THREAD_PRIORITY, 0, thread_handler, "2", "thread2");
    if (thread2_pid <= KERNEL_PID_UNDEF) {
        printf("Error: Unable to create thread 2\n");
        return 1;
    }

    sleep(3);

    // Change the priority of thread 1 at runtime
    thread_t *thread1 = thread_get(thread1_pid);
    if (thread1) {
        sched_change_priority(thread1, THREAD_PRIORITY_MAIN);
        printf("Thread 1 priority changed to %d\n", THREAD_PRIORITY_MAIN);
    }

    // Continues running, the thread_handler does not terminate so main does not either
    while (1) {
        sleep(10);
    }

    return 0;
}