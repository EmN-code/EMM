
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define TEST_VALUE 42

volatile atomic_t test_atomic = ATOMIC_INIT(TEST_VALUE);

void interrupt_handler(void)
{
    // Simulate an interrupt by modifying the atomic variable
    atomic_store(&test_atomic, TEST_VALUE + 1);
}

int main(void)
{
    // Disable interrupts initially
    unsigned state = irq_disable();

    // Modify the atomic variable in an atomic context
    atomic_store(&test_atomic, TEST_VALUE);

    // Verify the value before enabling interrupts
    if (atomic_load(&test_atomic) != TEST_VALUE) {
        puts("Error: Atomic variable modified unexpectedly.");
        return 1;
    }

    // Enable interrupts
    irq_restore(state);

    // Simulate an interrupt by calling the handler
    interrupt_handler();

    // Verify the value after the interrupt
    if (atomic_load(&test_atomic) != (TEST_VALUE + 1)) {
        puts("Error: Interrupt handler did not modify the atomic variable correctly.");
        return 1;
    }

    puts("Interrupt handling test passed.");
    return 0;
}
