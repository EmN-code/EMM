
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_types.h>

atomic_int counter = ATOMIC_INIT(0);

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

    // Increment the counter
    atomic_store(&counter, 0);

    // Restore interrupts
    irq_restore(state);

    // Trigger the interrupt
    irq_trigger(0);

    // Wait for the interrupt to be handled
    thread_sleep(1000000);

    // Check the counter value
    if (atomic_load(&counter) == 1) {
        printf("Interrupt handling test passed\n");
    } else {
        printf("Interrupt handling test failed\n");
    }

    return 0;
}
