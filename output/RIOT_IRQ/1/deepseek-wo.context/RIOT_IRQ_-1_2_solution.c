
#include "irq.h"
#include "thread.h"
#include "atomic_utils.h"

atomic_t atomic_var = ATOMIC_INIT(0);

void interrupt_handler(void)
{
    atomic_set(&atomic_var, 1);
}

int main(void)
{
    atomic_set(&atomic_var, 0);

    // Disable interrupts
    unsigned state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler);

    // Restore interrupts
    irq_restore(state);

    // Verify the atomic variable
    if (atomic_get(&atomic_var) == 1) {
        printf("Interrupts were disabled and restored correctly.\n");
    } else {
        printf("Interrupts were not disabled and restored correctly.\n");
    }

    return 0;
}
