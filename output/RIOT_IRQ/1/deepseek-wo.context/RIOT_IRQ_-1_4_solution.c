
#include <stdio.h>
#include <stdint.h>
#include <irq.h>
#include <atomic_utils.h>

// Define an atomic variable
atomic_uint_t atomic_var = ATOMIC_INIT(0);

// Interrupt handler function
void isr_handler(void *arg)
{
    (void)arg;
    printf("Interrupt triggered!\n");
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    // Disable interrupts
    unsigned state = irq_disable();

    // Set the atomic variable to 0
    atomic_store(&atomic_var, 0);

    // Restore interrupts
    irq_restore(state);

    // Trigger the interrupt
    isr_handler(NULL);

    // Check the atomic variable
    if (atomic_load(&atomic_var) == 1) {
        printf("Interrupts were disabled and restored correctly.\n");
    } else {
        printf("Interrupts were not handled correctly.\n");
    }

    return 0;
}
