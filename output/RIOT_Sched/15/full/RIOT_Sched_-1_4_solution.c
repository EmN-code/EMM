// Including necessary headers in descending alphabetical order by their names
#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "thread_config.h"
#include "thread_arch.h"
#include "thread.h"
#include "sys/socket.h"
#include "sched.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "arpa/inet.h"
#include "architecture.h"
#include "mutex.h"
#include "net/gcoap.h"

// Function declarations
void *thread_one(void *arg);
void *thread_two(void *arg);

// Function that updates the priority of a given thread
void update_priority(kernel_pid_t pid, uint8_t new_priority) {
    thread_t *thread = thread_get(pid);
    if (thread != NULL) {
        sched_change_priority(thread, new_priority);
    }
}

int main() {
    // Thread stacks
    char thread_one_stack[THREAD_STACKSIZE_MAIN];
    char thread_two_stack[THREAD_STACKSIZE_MAIN];
    
    // Create threads
    kernel_pid_t pid_one = thread_create(thread_one_stack, sizeof(thread_one_stack), THREAD_PRIORITY_MAIN - 1, 0, thread_one, NULL, "thread_one");
    kernel_pid_t pid_two = thread_create(thread_two_stack, sizeof(thread_two_stack), THREAD_PRIORITY_MAIN - 2, 0, thread_two, NULL, "thread_two");

    // Uncomment this line if you use pid_two somewhere else in the code
    // (void)pid_two;  // Fix unused variable warning by typecasting

    // Change priority at runtime (After some operation, here it's immediate for demo)
    update_priority(pid_one, THREAD_PRIORITY_MAIN - 2);

    // Keep the main thread alive to let the threads run
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}

void *thread_one(void *arg) {
    (void)arg;
    while (1) {
        // Do some work
        ztimer_sleep(ZTIMER_MSEC, 500);
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    while (1) {
        // Do some work
        ztimer_sleep(ZTIMER_MSEC, 500);
    }
    return NULL;
}