
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define TEST_VALUE 42

volatile atomic_int_t atomic_var = ATOMIC_INIT(TEST_VALUE);

void interrupt_handler(void)
{
    unsigned state = irq_disable();
    atomic_store(&atomic_var, atomic_load(&atomic_var) + 1);
    irq_restore(state);
}

int main(void)
{
    unsigned state;
    int initial_value, final_value;

    // Initialize the atomic variable
    atomic_store(&atomic_var, TEST_VALUE);

    // Disable interrupts and store the state
    state = irq_disable();

    // Modify the atomic variable while interrupts are disabled
    atomic_store(&atomic_var, atomic_load(&atomic_var) + 1);

    // Restore interrupts
    irq_restore(state);

    // Verify the value before enabling the interrupt handler
    initial_value = atomic_load(&atomic_var);

    // Enable the interrupt handler
    irq_set_handler(interrupt_handler);

    // Wait for a short period to allow the interrupt handler to be called
    xtimer_sleep(1);

    // Verify the value after the interrupt handler has been called
    final_value = atomic_load(&atomic_var);

    // Check if the interrupt handler modified the atomic variable
    if (final_value > initial_value) {
        printf("Interrupt handling test passed: %d -> %d\n", initial_value, final_value);
    } else {
        printf("Interrupt handling test failed: %d -> %d\n", initial_value, final_value);
    }

    return 0;
}
