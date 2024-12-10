#include "thread_config.h"
#include "thread_arch.h"
#include "thread.h"
#include "event/thread.h"

static char stack1[THREAD_STACKSIZE_MAIN];
static char stack2[THREAD_STACKSIZE_MAIN];

void *thread_handler(void *arg) {
    while (1) {
        thread_yield();
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread1 = thread_create(
        stack1,
        sizeof(stack1),
        thread_get_priority() - 1, // Lower priority than main
        THREAD_CREATE_STACKTEST,
        thread_handler,
        NULL,
        "thread1"
    );

    kernel_pid_t thread2 = thread_create(
        stack2,
        sizeof(stack2),
        thread_get_priority() + 1, // Higher priority than main
        THREAD_CREATE_STACKTEST,
        thread_handler,
        NULL,
        "thread2"
    );

    // Thread priorities are set during creation, yielding to allow scheduling
    thread_yield();

    return 0;
}