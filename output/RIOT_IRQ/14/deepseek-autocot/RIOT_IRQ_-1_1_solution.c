
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define TEST_VALUE 42

volatile atomic_int_t atomic_value = ATOMIC_INIT(0);

void interrupt_handler(void)
{
    atomic_store(&atomic_value, TEST_VALUE);
}

int main(void)
{
    unsigned state;

    // Initialize the atomic variable
    atomic_store(&atomic_value, 0);

    // Disable interrupts
    state = irq_disable();

    // Simulate some critical section code
    xtimer_sleep(1);

    // Restore interrupts
    irq_restore(state);

    // Trigger the interrupt handler
    interrupt_handler();

    // Verify the atomic variable value
    if (atomic_load(&atomic_value) == TEST_VALUE) {
        printf("Interrupt handling and atomic operations work correctly.\n");
    } else {
        printf("Interrupt handling or atomic operations failed.\n");
    }

    return 0;
}
