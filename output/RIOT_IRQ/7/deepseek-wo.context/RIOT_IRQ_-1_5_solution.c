
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

    // Initialize the atomic variable
    atomic_init(&flag, 0);

    // Set the interrupt handler
    irq_set_handler(IRQ_NUM, irq_handler);

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify the atomic variable while interrupts are disabled
    atomic_store(&flag, 0);

    // Restore interrupts
    irq_restore(state);

    // Trigger the interrupt
    irq_trigger(IRQ_NUM);

    // Wait for the interrupt to be handled
    thread_sleep(1000000);

    // Check the value of the atomic variable
    if (atomic_load(&flag) == 1) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt handling failed.\n");
    }

    return 0;
}
