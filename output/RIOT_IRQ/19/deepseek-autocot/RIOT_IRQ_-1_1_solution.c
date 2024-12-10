
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

    // Modify the atomic variable
    atomic_store(&test_atomic, TEST_VALUE + 1);

    // Restore interrupts
    irq_restore(state);

    // Verify the value before the interrupt
    initial_value = atomic_load(&test_atomic);

    // Trigger the interrupt
    interrupt_handler();

    // Verify the value after the interrupt
    final_value = atomic_load(&test_atomic);

    // Check if the interrupt handler modified the value correctly
    if (final_value == (TEST_VALUE + 1)) {
        printf("Interrupt handling and atomic variable modification successful!\n");
    } else {
        printf("Interrupt handling failed!\n");
    }

    return 0;
}
