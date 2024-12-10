
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define TEST_VALUE 42

volatile atomic_t interrupt_flag = ATOMIC_INIT(0);

void interrupt_handler(void)
{
    atomic_store(&interrupt_flag, 1);
}

int main(void)
{
    unsigned state;

    // Initialize the interrupt flag
    atomic_store(&interrupt_flag, 0);

    // Disable interrupts
    state = irq_disable();

    // Perform some critical section code
    atomic_store(&interrupt_flag, TEST_VALUE);

    // Restore interrupts
    irq_restore(state);

    // Simulate an interrupt
    interrupt_handler();

    // Check if the interrupt flag was set correctly
    if (atomic_load(&interrupt_flag) == 1) {
        printf("Interrupt handling works correctly.\n");
    } else {
        printf("Interrupt handling failed.\n");
    }

    return 0;
}
