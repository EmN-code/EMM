
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int_t atomic_var;

void interrupt_handler(void *arg)
{
    (void)arg;
    printf("Interrupt occurred!\n");
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Initialize the atomic variable
    atomic_init(&atomic_var, 0);

    // Register the interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some critical section code
    printf("Critical section: Disabling interrupts...\n");
    thread_sleep(1000000); // Simulate some work

    // Restore interrupts
    irq_restore(state);
    printf("Critical section: Restoring interrupts...\n");

    // Wait for an interrupt to occur
    while (atomic_load(&atomic_var) == 0) {
        thread_yield();
    }

    printf("Interrupt handling test completed.\n");
    return 0;
}
