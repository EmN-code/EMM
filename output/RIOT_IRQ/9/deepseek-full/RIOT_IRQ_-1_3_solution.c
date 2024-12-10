
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "irq.h"
#include "thread.h"
#include "event/thread.h"

#define STACKSIZE 512
#define PRIORITY 5

static char stack[STACKSIZE];
static volatile int atomic_var = 0;

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_var = 1; // Simulate atomic variable modification
}

int main(void)
{
    kernel_pid_t pid;
    int irq_num = 0; // Example IRQ number

    // Create a thread
    pid = thread_create(stack, STACKSIZE, PRIORITY, THREAD_CREATE_STACKTEST, interrupt_handler, NULL, "interrupt_thread");
    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
        return 1;
    }

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify atomic variable
    atomic_var = 0;

    // Restore interrupts
    irq_restore(state);

    // Verify atomic variable
    if (atomic_var == 0) {
        printf("Atomic variable correctly not modified during interrupt disable\n");
    } else {
        printf("Atomic variable incorrectly modified during interrupt disable\n");
    }

    // Simulate interrupt by waking up the thread
    thread_wakeup(pid);

    // Final verification
    if (atomic_var == 1) {
        printf("Interrupt handling was successful\n");
    } else {
        printf("Interrupt handling failed\n");
    }

    return 0;
}
