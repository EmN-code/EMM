
#include "irq.h"
#include "xtimer.h"
#include "thread.h"
#include "atomic_utils.h"

#define IRQ_NUM 0  // Define the IRQ number to be used

volatile atomic_int_t atomic_var = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    // Initialize the IRQ handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some atomic operations
    atomic_store(&atomic_var, 0);

    // Restore interrupts
    irq_restore(state);

    // Enable the IRQ
    irq_enable(IRQ_NUM);

    // Wait for the IRQ to be handled
    xtimer_sleep(1);

    // Check the atomic variable
    if (atomic_load(&atomic_var) == 1) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt was not handled.\n");
    }

    return 0;
}
