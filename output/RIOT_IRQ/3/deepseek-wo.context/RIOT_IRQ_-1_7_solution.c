
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

volatile atomic_t interrupt_flag = ATOMIC_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_store(&interrupt_flag, 1);
}

int main(void)
{
    // Initialize the interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some critical section code
    atomic_store(&interrupt_flag, 0);

    // Restore interrupts
    irq_restore(state);

    // Wait for the interrupt to be handled
    while (atomic_load(&interrupt_flag) == 0) {
        xtimer_sleep(1);
    }

    // Verify if the interrupt was handled
    if (atomic_load(&interrupt_flag) == 1) {
        printf("Interrupt was successfully handled!\n");
    } else {
        printf("Interrupt handling failed!\n");
    }

    return 0;
}
