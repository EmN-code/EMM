
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int_t counter = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Set the interrupt handler
    irq_set_handler(0, irq_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Trigger the interrupt
    irq_trigger(0);

    // Restore interrupts
    irq_restore(state);

    // Check the atomic variable
    if (atomic_load(&counter) == 1) {
        printf("Interrupt was handled correctly\n");
    } else {
        printf("Interrupt handling failed\n");
    }

    return 0;
}
