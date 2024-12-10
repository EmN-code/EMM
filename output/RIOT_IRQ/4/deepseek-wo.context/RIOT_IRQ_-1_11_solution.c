
#include <stdio.h>
#include <irq.h>
#include <atomic_utils.h>

atomic_int_t atomic_var;

void irq_handler(void *arg)
{
    (void)arg;
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

    // Wait for the interrupt to occur
    while (atomic_load(&atomic_var) == 0) {
        // Busy wait
    }

    printf("Interrupt handled successfully!\n");

    return 0;
}
