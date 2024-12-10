
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

#define IRQ_NUM 0  // Define the IRQ number to use

atomic_t flag = ATOMIC_INIT(0);  // Initialize atomic variable

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&flag, 1);  // Set the atomic flag to 1
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Set the IRQ handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify the atomic variable while interrupts are disabled
    atomic_store(&flag, 0);

    // Restore interrupts
    irq_restore(state);

    // Trigger the interrupt
    irq_set(IRQ_NUM, 1);

    // Wait for a short period to allow the interrupt to be handled
    thread_sleep(100000);

    // Check the atomic variable
    if (atomic_load(&flag) == 1) {
        printf("Interrupt was handled correctly\n");
    } else {
        printf("Interrupt was not handled correctly\n");
    }

    return 0;
}
