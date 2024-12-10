
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define TEST_VALUE 42

volatile atomic_t atomic_value = ATOMIC_INIT(TEST_VALUE);

void interrupt_handler(void)
{
    unsigned state = irq_disable();
    atomic_store(&atomic_value, TEST_VALUE + 1);
    irq_restore(state);
}

int main(void)
{
    unsigned state;

    // Initialize the atomic variable
    atomic_store(&atomic_value, TEST_VALUE);

    // Disable interrupts and store the state
    state = irq_disable();

    // Modify the atomic variable
    atomic_store(&atomic_value, TEST_VALUE + 1);

    // Restore interrupts
    irq_restore(state);

    // Verify the value of the atomic variable
    if (atomic_load(&atomic_value) == TEST_VALUE + 1) {
        printf("Interrupts were disabled and restored correctly.\n");
    } else {
        printf("Interrupts were not handled correctly.\n");
    }

    // Simulate an interrupt by calling the handler manually
    interrupt_handler();

    // Verify the value of the atomic variable again
    if (atomic_load(&atomic_value) == TEST_VALUE + 2) {
        printf("Interrupt handler executed correctly.\n");
    } else {
        printf("Interrupt handler did not execute correctly.\n");
    }

    return 0;
}
