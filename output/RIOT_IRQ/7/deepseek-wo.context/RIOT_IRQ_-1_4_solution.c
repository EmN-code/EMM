
#include <stdio.h>
#include <stdint.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

#define IRQ_NUM 0  // Define the IRQ number to use

atomic_t flag;  // Use atomic_t from atomic_utils.h

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&flag, 1);  // Store 1 into the atomic variable
}

int main(void)
{
    atomic_init(&flag, 0);  // Initialize the atomic variable to 0

    // Set the IRQ handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Disable interrupts to test atomic operations
    unsigned state = irq_disable();

    // Modify the atomic variable while interrupts are disabled
    atomic_store(&flag, 2);

    // Restore interrupts
    irq_restore(state);

    // Load the value of the atomic variable
    uint8_t value = atomic_load(&flag);

    printf("Atomic variable value: %u\n", value);

    return 0;
}
