
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
    puts("Starting interrupt handling test...");

    // Disable interrupts initially
    unsigned state = irq_disable();

    // Modify the atomic variable while interrupts are disabled
    atomic_store(&atomic_var, atomic_load(&atomic_var) + 1);

    // Restore interrupts
    irq_restore(state);

    // Simulate an interrupt by calling the handler manually
    interrupt_handler();

    // Check the value of the atomic variable
    if (atomic_load(&atomic_var) == TEST_VALUE + 2) {
        puts("Interrupt handling test passed!");
    } else {
        puts("Interrupt handling test failed!");
    }

    return 0;
}
