
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define TEST_VALUE 42

volatile atomic_int_t atomic_value = ATOMIC_INIT(TEST_VALUE);

void interrupt_handler(void)
{
    // Simulate an interrupt by modifying the atomic variable
    atomic_store(&atomic_value, TEST_VALUE + 1);
}

int main(void)
{
    // Disable interrupts initially
    unsigned state = irq_disable();

    // Modify the atomic variable in an atomic context
    atomic_store(&atomic_value, TEST_VALUE);

    // Restore interrupts
    irq_restore(state);

    // Simulate an interrupt by calling the handler
    interrupt_handler();

    // Verify the value after the interrupt
    int value = atomic_load(&atomic_value);
    if (value == TEST_VALUE + 1) {
        printf("Interrupt handling and atomic operations work correctly.\n");
    } else {
        printf("Interrupt handling or atomic operations failed.\n");
    }

    return 0;
}
