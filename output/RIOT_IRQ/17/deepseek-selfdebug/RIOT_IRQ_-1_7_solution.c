
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_t flag = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&flag, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Set the IRQ handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Set the flag to 0
    atomic_store(&flag, 0);

    // Enable interrupts
    irq_restore(state);

    // Trigger the interrupt
    irq_set(IRQ_NUM);

    // Wait for the interrupt to be handled
    thread_sleep();

    // Check the flag
    if (atomic_load(&flag) == 1) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt handling failed.\n");
    }

    return 0;
}
