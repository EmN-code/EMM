
#include "event/thread.h"
#include "irq.h"
#include "thread.h"
#include "atomic_utils.h"  // Assuming atomic utilities are provided in this header

#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static atomic_t interrupt_handled = ATOMIC_INIT(0);

static void *interrupt_handler(void *arg)
{
    (void)arg;
    atomic_store(&interrupt_handled, 1);
    return NULL;
}

int main(void)
{
    unsigned state = irq_disable();

    // Create an event thread for handling interrupts
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY,
                  THREAD_CREATE_STACKTEST, interrupt_handler, NULL, "interrupt_handler");

    // Simulate some work
    printf("Simulating work...\n");
    thread_sleep(1000000);  // Sleep for 1 second

    // Restore previous interrupt state
    irq_restore(state);

    // Check if the interrupt was handled
    if (atomic_load(&interrupt_handled)) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt was not handled.\n");
    }

    return 0;
}
