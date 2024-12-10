
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

    // Disable interrupts and save the state
    state = irq_disable();

    // Simulate some critical section code
    xtimer_sleep(1);

    // Verify that interrupts are disabled
    if (irq_is_in()) {
        puts("Interrupts are still enabled inside critical section!");
    } else {
        puts("Interrupts are disabled inside critical section.");
    }

    // Restore the interrupt state
    irq_restore(state);

    // Verify that interrupts are restored
    if (irq_is_in()) {
        puts("Interrupts are enabled after restoring state.");
    } else {
        puts("Interrupts are still disabled after restoring state!");
    }

    // Set up an interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler);
    irq_enable(IRQ_NUM);

    // Trigger the interrupt
    irq_trigger(IRQ_NUM);

    // Wait for the interrupt to be handled
    xtimer_sleep(1);

    // Check the interrupt flag
    if (atomic_load(&interrupt_flag) == 1) {
        puts("Interrupt was handled correctly.");
    } else {
        puts("Interrupt was not handled!");
    }

    return 0;
}
