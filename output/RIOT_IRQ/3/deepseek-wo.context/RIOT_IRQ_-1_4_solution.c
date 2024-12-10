
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int counter = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Set the interrupt handler
    irq_set_handler(0, irq_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some operations while interrupts are disabled
    printf("Interrupts are disabled. Performing operations...\n");

    // Restore interrupts
    irq_restore(state);

    // Wait for the interrupt to be handled
    thread_sleep();

    // Check the atomic variable to verify the interrupt was handled
    if (atomic_load(&counter) == 1) {
        printf("Interrupt was successfully handled.\n");
    } else {
        printf("Interrupt handling failed.\n");
    }

    return 0;
}
