
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_t counter = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    atomic_add(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Set the IRQ handler
    irq_set_handler(0, irq_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Increment the counter atomically
    atomic_add(&counter, 1);

    // Restore interrupts
    irq_restore(state);

    // Enable interrupts
    irq_enable();

    // Wait for an interrupt to occur
    thread_sleep();

    // Check the counter value
    if (atomic_get(&counter) == 2) {
        printf("Interrupt handling test passed!\n");
    } else {
        printf("Interrupt handling test failed!\n");
    }

    return 0;
}
