
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define TEST_VALUE 0x12345678

volatile uint32_t atomic_var = TEST_VALUE;

void interrupt_handler(void)
{
    // Simulate an interrupt by modifying the atomic variable
    atomic_var = TEST_VALUE + 1;
}

int main(void)
{
    // Disable interrupts initially
    unsigned state = irq_disable();

    // Set up the interrupt handler
    irq_set_handler(interrupt_handler);

    // Enable interrupts
    irq_restore(state);

    // Verify that interrupts are enabled
    if (!irq_is_enabled()) {
        puts("Interrupts are not enabled!");
        return 1;
    }

    // Simulate some work
    xtimer_sleep(1);

    // Check the atomic variable to verify the interrupt handler was called
    if (atomic_var == (TEST_VALUE + 1)) {
        puts("Interrupt handling and atomic variable modification successful!");
    } else {
        puts("Interrupt handling failed!");
    }

    return 0;
}
