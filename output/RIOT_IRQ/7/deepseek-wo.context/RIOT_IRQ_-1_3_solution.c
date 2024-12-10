
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

    // Simulate an interrupt by manually calling the handler
    interrupt_handler();

    // Check the flag to verify the interrupt was handled
    if (atomic_load(&flag) == 1) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt handling failed.\n");
    }

    // Restore interrupts to their previous state
    irq_restore(state);

    printf("Interrupt handling test completed.\n");

    return 0;
}
