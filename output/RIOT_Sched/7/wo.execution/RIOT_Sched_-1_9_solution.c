#include "netinet/in.h"
#include "net_gcoap.h"
#include "net_emcute.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "msg.h"
#include "mbox.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "architecture.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "thread_arch.h"
#include "thread_config.h"

#define UNUSED(x) (void)(x)
#define THREAD_STACKSIZE_MAIN    (KERNEL_STACKSIZE / 2)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)

static char stack_one[THREAD_STACKSIZE_MAIN];
static char stack_two[THREAD_STACKSIZE_MAIN];

static void *thread_func_one(void *arg) {
    UNUSED(arg);
    while (1) {
        // Thread one functionality
    }
    return NULL;
}

static void *thread_func_two(void *arg) {
    UNUSED(arg);
    while (1) {
        // Thread two functionality
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid_one, pid_two;

    pid_one = thread_create(stack_one, sizeof(stack_one),
                            THREAD_PRIORITY_MAIN - 1, 0,
                            thread_func_one, NULL, "thread_one");

    pid_two = thread_create(stack_two, sizeof(stack_two),
                            THREAD_PRIORITY_MAIN - 2, 0,
                            thread_func_two, NULL, "thread_two");

    // Change priority of threads at runtime
    thread_t *thread_one = thread_get(pid_one);
    thread_t *thread_two = thread_get(pid_two);

    if (thread_one != NULL && thread_two != NULL) {
        // Increase priority of thread one
        sched_change_priority(thread_one, THREAD_PRIORITY_MAIN + 1);
        // Decrease priority of thread two
        sched_change_priority(thread_two, THREAD_PRIORITY_MAIN - 3);
    }

    return 0;
}