#include "net/sock/udp.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "sys/socket.h"
#include "time_units.h"
#include "thread_arch.h"
#include "thread_config.h"
#include "thread.h"
#include "sched.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "arpa/inet.h"
#include "architecture.h"

#define MY_STACKSIZE (THREAD_STACKSIZE_MAIN + 256)

static char thread_stack_one[MY_STACKSIZE];
static char thread_stack_two[MY_STACKSIZE];

void *thread_one(void *arg) {
    (void)arg;
    while (1) {
        // Do something
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    while (1) {
        // Do something different
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one = thread_create(thread_stack_one, sizeof(thread_stack_one), THREAD_PRIORITY_MAIN - 1, 0, thread_one, NULL, "thread_one");
    kernel_pid_t pid_two = thread_create(thread_stack_two, sizeof(thread_stack_two), THREAD_PRIORITY_MAIN - 2, 0, thread_two, NULL, "thread_two");

    // Change priority of thread_one at runtime
    sched_change_priority(thread_get(pid_one), THREAD_PRIORITY_MAIN - 2);

    while (1) {
        // main thread sleep or does something
    }
    return 0;
}