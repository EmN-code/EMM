
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int counter = ATOMIC_VAR_INIT(0);

void irq_handler(void)
{
    atomic_store(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Set the interrupt handler
    irq_set_handler(irq_handler);

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some critical section code
    atomic_store(&counter, 0);

    // Restore interrupts
    irq_restore(state);

    // Enable interrupts
    irq_enable();

    // Wait for an interrupt to occur
    while (atomic_load(&counter) == 0) {
        thread_yield();
    }

    printf("Interrupt occurred, counter value: %d\n", atomic_load(&counter));

    return 0;
}
