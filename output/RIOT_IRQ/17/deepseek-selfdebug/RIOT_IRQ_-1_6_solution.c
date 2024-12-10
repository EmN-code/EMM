
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_t flag = ATOMIC_INIT(0);

void irq_handler(void)
{
    atomic_store(&flag, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(0, irq_handler);

    // Restore interrupts
    irq_restore(state);

    // Trigger the interrupt
    irq_set(0);

    // Wait for the interrupt to be handled
    while (atomic_load(&flag) == 0) {
        thread_yield();
    }

    printf("Interrupt handled successfully!\n");

    return 0;
}
