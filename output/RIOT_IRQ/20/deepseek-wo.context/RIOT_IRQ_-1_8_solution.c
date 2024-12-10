
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

#define IRQ_NUM 0  // Define the IRQ number to use

atomic_t flag = ATOMIC_INIT(0);  // Initialize atomic variable

void irq_handler(void *arg)
{
    (void)arg;  // Unused parameter

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify the atomic variable
    atomic_store(&flag, 1);

    // Restore interrupts
    irq_restore(state);
}

int main(void)
{
    // Set the IRQ handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Trigger the IRQ to test the handler
    irq_enable(IRQ_NUM);

    // Wait for the IRQ to be handled
    thread_sleep();

    // Check the atomic variable
    if (atomic_load(&flag) == 1) {
        printf("Interrupt handling and atomic operations worked correctly.\n");
    } else {
        printf("Interrupt handling or atomic operations failed.\n");
    }

    return 0;
}
