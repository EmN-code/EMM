
#include "event/thread.h"
#include "thread.h"
#include "irq.h"

#define THREAD_STACKSIZE (512)

static char thread_stack[THREAD_STACKSIZE];
static kernel_pid_t thread_pid;

volatile int atomic_var = 0;

void *thread_handler(void *arg)
{
    (void)arg;

    // Simulate interrupt handling
    atomic_var = 1;

    return NULL;
}

int main(void)
{
    // Create a thread to simulate interrupt handling
    thread_pid = thread_create(thread_stack, sizeof(thread_stack),
                               THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST,
                               thread_handler, NULL, "interrupt_handler");

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify atomic variable
    atomic_var = 0;

    // Restore interrupts
    irq_restore(state);

    // Verify atomic variable
    if (atomic_var == 0) {
        puts("Interrupts were disabled correctly.");
    } else {
        puts("Interrupts were not disabled correctly.");
    }

    // Simulate an interrupt
    thread_wakeup(thread_pid);

    // Final verification of atomic variable
    if (atomic_var == 1) {
        puts("Interrupt was handled correctly.");
    } else {
        puts("Interrupt was not handled correctly.");
    }

    return 0;
}
