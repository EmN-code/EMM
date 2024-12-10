
#include "irq.h"
#include "atomic_utils.h"
#include "thread.h"

atomic_t flag = ATOMIC_INIT(0);

void my_irq_handler(void)
{
    atomic_store(&flag, 1);
}

int main(void)
{
    unsigned state;

    // Disable interrupts
    state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(my_irq_handler);

    // Restore interrupts
    irq_restore(state);

    // Simulate an interrupt occurring
    irq_set(1);

    // Check the flag to verify the interrupt handler was called
    if (atomic_load(&flag) == 1) {
        printf("Interrupt handling works correctly!\n");
    } else {
        printf("Interrupt handling failed!\n");
    }

    return 0;
}
