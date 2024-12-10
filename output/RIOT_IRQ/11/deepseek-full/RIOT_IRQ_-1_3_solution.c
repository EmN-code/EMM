
#include "event/thread.h"
#include "stdio.h"
#include "irq.h"
#include "thread.h"
#include "stdbool.h"
#include "MQTTClient.h"
#include "stdlib.h"
#include "stddef.h"

#define THREAD_STACKSIZE_DEFAULT (1024)

static volatile int atomic_var = 0;

void interrupt_handler(void *arg) {
    (void)arg;
    atomic_var = 1;
}

void test_thread(void *arg) {
    (void)arg;
    irq_disable();
    atomic_var = 0;
    irq_restore();

    // Simulate some work
    for (int i = 0; i < 1000000; i++) {
        // Busy loop
    }

    if (atomic_var == 1) {
        puts("Interrupt was handled correctly.");
    } else {
        puts("Interrupt was not handled correctly.");
    }

    thread_exit();
}

int main(void) {
    char *stack = malloc(THREAD_STACKSIZE_DEFAULT);
    if (stack == NULL) {
        puts("Failed to allocate stack for thread.");
        return 1;
    }

    kernel_pid_t pid = thread_create(stack, THREAD_STACKSIZE_DEFAULT,
                                     THREAD_PRIORITY_MAIN - 1, 0,
                                     test_thread, NULL, "test_thread");
    if (pid <= KERNEL_PID_UNDEF) {
        puts("Failed to create thread.");
        free(stack);
        return 1;
    }

    // Simulate an interrupt
    interrupt_handler(NULL);

    return 0;
}
