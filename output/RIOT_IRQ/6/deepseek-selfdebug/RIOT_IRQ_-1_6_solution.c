
#include <stdio.h>
#include <irq.h>
#include <thread_flags.h>
#include <atomic_utils.h>

atomic_t counter = ATOMIC_INIT(0);

void irq_handler(void)
{
    atomic_fetch_add(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(IRQ_NUM, irq_handler);

    // Enable interrupts
    irq_restore(state);

    // Simulate some work
    for (int i = 0; i < 1000000; i++) {
        // Do some work
    }

    // Check the counter value
    int final_count = atomic_load(&counter);
    printf("Final counter value: %d\n", final_count);

    return 0;
}
