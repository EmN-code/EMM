
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_t counter = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    (void)arg;
    atomic_add(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Set the interrupt handler
    irq_set_handler(0, irq_handler, NULL);

    // Enable the interrupt
    irq_enable(0);

    // Disable interrupts to test atomic operations
    unsigned state = irq_disable();

    // Perform an atomic operation
    atomic_add(&counter, 1);

    // Restore the interrupt state
    irq_restore(state);

    // Check the counter value
    printf("Counter value: %d\n", atomic_get(&counter));

    return 0;
}
