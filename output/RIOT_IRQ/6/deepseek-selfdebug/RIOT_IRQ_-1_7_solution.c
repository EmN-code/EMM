
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int counter = ATOMIC_VAR_INIT(0);

void irq_handler(void)
{
    atomic_fetch_add(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Set the IRQ handler
    irq_set_handler(IRQ_NUM, irq_handler);

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some operations while interrupts are disabled
    for (int i = 0; i < 1000000; i++) {
        // Simulate work
    }

    // Restore interrupts to their previous state
    irq_restore(state);

    // Check the counter value
    int final_count = atomic_load(&counter);
    printf("Counter value after interrupt handling: %d\n", final_count);

    return 0;
}
