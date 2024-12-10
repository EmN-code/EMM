
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define TEST_VALUE 0x12345678

volatile uint32_t atomic_var = TEST_VALUE;

void interrupt_handler(void)
{
    // Simulate an interrupt by modifying the atomic variable
    atomic_var = TEST_VALUE ^ 0xFFFFFFFF;
}

int main(void)
{
    unsigned state;

    // Initialize the atomic variable
    atomic_var = TEST_VALUE;

    // Disable interrupts
    state = irq_disable();

    // Simulate some critical section code
    atomic_var = TEST_VALUE;

    // Re-enable interrupts
    irq_restore(state);

    // Trigger the interrupt handler
    interrupt_handler();

    // Check if the atomic variable was modified correctly
    if (atomic_var == (TEST_VALUE ^ 0xFFFFFFFF)) {
        printf("Interrupt handling and atomic variable modification successful!\n");
    } else {
        printf("Interrupt handling or atomic variable modification failed!\n");
    }

    return 0;
}
