
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_t flag = ATOMIC_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_store(&flag, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Set the interrupt handler
    irq_set_handler(interrupt_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some critical section code
    printf("Critical section: Disabling interrupts...\n");

    // Restore interrupts
    irq_restore(state);

    // Wait for the interrupt to be handled
    while (atomic_load(&flag) == 0) {
        thread_yield();
    }

    printf("Interrupt handled, flag set to 1.\n");

    return 0;
}
