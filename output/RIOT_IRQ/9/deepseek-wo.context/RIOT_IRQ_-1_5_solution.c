
#include <stdio.h>
#include <irq.h>
#include <atomic.h>

atomic_int counter = ATOMIC_VAR_INIT(0);

void irq_handler(void *arg)
{
    atomic_store(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Set the IRQ handler
    irq_set_handler(0, irq_handler, NULL);

    // Trigger the IRQ
    irq_trigger(0);

    // Wait for the interrupt to be handled
    while (atomic_load(&counter) == 0) {
        // Busy wait
    }

    printf("Interrupt handled, counter value: %d\n", atomic_load(&counter));

    return 0;
}
