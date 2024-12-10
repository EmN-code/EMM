
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

    // Disable interrupts and save the current state
    state = irq_disable();

    // Perform some critical section code
    printf("Critical section: Interrupt flag is %d\n", atomic_load(&interrupt_flag));

    // Simulate an interrupt occurring during the critical section
    interrupt_handler();

    // Verify that the interrupt flag has not changed
    if (atomic_load(&interrupt_flag) == 0) {
        printf("Interrupts were disabled correctly.\n");
    } else {
        printf("Interrupts were not disabled correctly.\n");
    }

    // Restore the interrupt state
    irq_restore(state);

    // Now that interrupts are restored, simulate another interrupt
    interrupt_handler();

    // Verify that the interrupt flag has changed
    if (atomic_load(&interrupt_flag) == 1) {
        printf("Interrupts were restored correctly.\n");
    } else {
        printf("Interrupts were not restored correctly.\n");
    }

    return 0;
}
