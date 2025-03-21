
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <xtimer.h>
#include <atomic_utils.h>

atomic_t flag = ATOMIC_INIT(0);

void interrupt_handler(void)
{
    atomic_store(&flag, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Set up an interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler);

    // Simulate an interrupt
    irq_set(IRQ_NUM);

    // Restore interrupts
    irq_restore(state);

    // Wait for the interrupt to be handled
    xtimer_sleep(1);

    // Check if the interrupt was handled
    if (atomic_load(&flag) == 1) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt was not handled.\n");
    }

    return 0;
}
