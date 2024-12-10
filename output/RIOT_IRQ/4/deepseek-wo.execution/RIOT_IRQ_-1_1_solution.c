
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define TEST_VALUE 42

volatile atomic_t atomic_var = ATOMIC_INIT(TEST_VALUE);

void interrupt_handler(void)
{
    unsigned state = irq_disable();
    atomic_var = ATOMIC_INIT(TEST_VALUE + 1);
    irq_restore(state);
}

int main(void)
{
    unsigned state;

    // Disable interrupts initially
    state = irq_disable();

    // Modify the atomic variable
    atomic_var = ATOMIC_INIT(TEST_VALUE);

    // Restore interrupts
    irq_restore(state);

    // Simulate an interrupt
    interrupt_handler();

    // Check if the interrupt handler modified the atomic variable correctly
    if (atomic_var == (TEST_VALUE + 1)) {
        printf("Interrupt handling and atomic variable modification successful!\n");
    } else {
        printf("Interrupt handling or atomic variable modification failed!\n");
    }

    return 0;
}
