
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_t counter = ATOMIC_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_add(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Register an interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some operations that should not be interrupted
    for (int i = 0; i < 1000; i++) {
        atomic_add(&counter, 1);
    }

    // Restore interrupts to their previous state
    irq_restore(state);

    // Trigger the interrupt
    irq_trigger(IRQ_NUM);

    // Wait for the interrupt handler to complete
    thread_sleep();

    // Check the counter value
    if (atomic_get(&counter) == 1001) {
        printf("Interrupt handling test passed!\n");
    } else {
        printf("Interrupt handling test failed! Counter value: %d\n", atomic_get(&counter));
    }

    return 0;
}
