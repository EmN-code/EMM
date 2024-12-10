
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define TEST_VALUE 42

volatile atomic_t test_atomic = ATOMIC_INIT(TEST_VALUE);

void interrupt_handler(void)
{
    unsigned state = irq_disable();
    atomic_store(&test_atomic, TEST_VALUE + 1);
    irq_restore(state);
}

int main(void)
{
    unsigned state;
    int initial_value, final_value;

    // Initialize the atomic variable
    atomic_store(&test_atomic, TEST_VALUE);

    // Disable interrupts and store the state
    state = irq_disable();

    // Modify the atomic variable while interrupts are disabled
    atomic_store(&test_atomic, TEST_VALUE + 1);

    // Restore interrupts
    irq_restore(state);

    // Verify the value before the interrupt handler is called
    initial_value = atomic_load(&test_atomic);

    // Simulate an interrupt by calling the handler manually
    interrupt_handler();

    // Verify the value after the interrupt handler is called
    final_value = atomic_load(&test_atomic);

    // Check if the interrupt handler modified the atomic variable correctly
    if (final_value == (TEST_VALUE + 2)) {
        printf("Interrupt handling test passed: %d -> %d\n", initial_value, final_value);
    } else {
        printf("Interrupt handling test failed: %d -> %d\n", initial_value, final_value);
    }

    return 0;
}
