
#include <stdio.h>
#include <stdint.h>
#include "irq.h"
#include "atomic_utils.h"

// Define an atomic variable
atomic_bit_u8_t atomic_var = ATOMIC_INIT(0);

void interrupt_handler(void)
{
    // Simulate an interrupt handler
    printf("Interrupt occurred!\n");
    atomic_store_u8(&atomic_var, 1);
}

int main(void)
{
    // Initialize the atomic variable
    atomic_store_u8(&atomic_var, 0);

    // Disable interrupts
    unsigned state = irq_disable();

    // Simulate some critical section code
    printf("Entering critical section...\n");

    // Check if the atomic variable is still 0
    uint8_t value = atomic_load_u8(&atomic_var);
    if (value == 0) {
        printf("Atomic variable is still 0, disabling interrupts worked!\n");
    } else {
        printf("Atomic variable changed to %d, disabling interrupts failed!\n", value);
    }

    // Restore interrupts
    irq_restore(state);

    // Enable interrupts and trigger the interrupt handler
    irq_enable();
    interrupt_handler();

    // Check the atomic variable after interrupt
    value = atomic_load_u8(&atomic_var);
    if (value == 1) {
        printf("Atomic variable changed to 1, interrupt handler worked!\n");
    } else {
        printf("Atomic variable is still %d, interrupt handler failed!\n", value);
    }

    return 0;
}
