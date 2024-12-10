
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define TEST_VALUE 0x12345678

volatile uint32_t atomic_var = TEST_VALUE;

void interrupt_handler(void)
{
    // Simulate an interrupt by modifying the atomic variable
    atomic_var = 0x87654321;
}

int main(void)
{
    // Initialize the atomic variable
    atomic_var = TEST_VALUE;

    // Disable interrupts
    unsigned state = irq_disable();

    // Simulate some critical section code
    atomic_var = 0x55555555;

    // Re-enable interrupts
    irq_restore(state);

    // Verify that the atomic variable was not modified by an interrupt
    if (atomic_var == 0x55555555) {
        printf("Interrupts were disabled correctly.\n");
    } else {
        printf("Interrupts were not disabled correctly.\n");
    }

    // Enable interrupts again
    state = irq_disable();
    irq_restore(state);

    // Simulate an interrupt occurring
    interrupt_handler();

    // Verify that the atomic variable was modified by the interrupt
    if (atomic_var == 0x87654321) {
        printf("Interrupts were enabled correctly.\n");
    } else {
        printf("Interrupts were not enabled correctly.\n");
    }

    return 0;
}
