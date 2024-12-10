
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int counter = ATOMIC_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_fetch_add(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Register an interrupt handler
    irq_set_handler(0, interrupt_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some operations that should not be interrupted
    for (int i = 0; i < 1000; i++) {
        atomic_fetch_add(&counter, 1);
    }

    // Restore interrupts to their previous state
    irq_restore(state);

    // Trigger the interrupt
    irq_trigger(0);

    // Wait for the interrupt to be handled
    thread_sleep(100000);

    // Check the counter value
    if (atomic_load(&counter) == 1001) {
        printf("Interrupt handling test passed!\n");
    } else {
        printf("Interrupt handling test failed!\n");
    }

    return 0;
}
