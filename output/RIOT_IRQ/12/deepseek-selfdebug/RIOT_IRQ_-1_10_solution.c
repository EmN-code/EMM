
#include <stdio.h>
#include <irq.h>
#include <atomic_utils.h>

atomic_int_t atomic_var;

void irq_handler(void *arg)
{
    printf("Interrupt occurred!\n");
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Initialize the atomic variable
    atomic_init(&atomic_var, 0);

    // Set the interrupt handler
    irq_set_handler(0, irq_handler, NULL);

    // Enable the interrupt
    irq_set(0, 1);

    // Disable interrupts
    unsigned state = irq_disable();

    // Check the atomic variable
    if (atomic_load(&atomic_var) == 0) {
        printf("Interrupts disabled correctly.\n");
    } else {
        printf("Interrupts were not disabled correctly.\n");
    }

    // Restore interrupts
    irq_restore(state);

    // Wait for an interrupt to occur
    while (atomic_load(&atomic_var) == 0) {
        // Busy wait
    }

    printf("Interrupt handling test completed.\n");

    return 0;
}
