
#include <stdio.h>
#include "irq.h"
#include "thread.h"
#include "atomic_utils.h"

atomic_int_t atomic_var = ATOMIC_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    printf("Interrupt occurred!\n");
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(interrupt_handler, NULL);

    // Restore interrupts
    irq_restore(state);

    // Enable interrupts
    irq_enable();

    // Wait for interrupt to occur
    while (atomic_load(&atomic_var) == 0) {
        thread_yield();
    }

    printf("Interrupt handling test completed.\n");

    return 0;
}
