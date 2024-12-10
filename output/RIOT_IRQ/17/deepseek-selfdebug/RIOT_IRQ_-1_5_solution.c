
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

    // Set up the interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler);

    // Simulate an interrupt by manually calling the handler
    interrupt_handler();

    // Restore interrupts
    irq_restore(state);

    // Check the flag to verify the interrupt was handled
    if (atomic_load(&flag) == 1) {
        printf("Interrupt was successfully handled.\n");
    } else {
        printf("Interrupt handling failed.\n");
    }

    return 0;
}
