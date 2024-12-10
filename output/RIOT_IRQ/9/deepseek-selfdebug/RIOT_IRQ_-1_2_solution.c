
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int_t counter = ATOMIC_INIT(0);

void interrupt_handler(void)
{
    atomic_store(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Simulate some work
    for (int i = 0; i < 1000000; i++) {
        // Do nothing
    }

    // Enable interrupts
    irq_restore(state);

    // Trigger the interrupt handler
    interrupt_handler();

    // Check the atomic variable
    if (atomic_load(&counter) == 1) {
        printf("Interrupts were disabled and restored correctly.\n");
    } else {
        printf("Interrupts were not handled correctly.\n");
    }

    return 0;
}
