
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_types.h>

atomic_int_t atomic_var = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    puts("Starting interrupt handling test...");

    // Set the interrupt handler
    irq_set_handler(0, irq_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify the atomic variable
    atomic_store(&atomic_var, 0);

    // Restore interrupts
    irq_restore(state);

    // Trigger the interrupt
    irq_set(0);

    // Wait for the interrupt to be handled
    thread_sleep();

    // Check the atomic variable
    int value = atomic_load(&atomic_var);
    if (value == 1) {
        puts("Interrupt handling test passed.");
    } else {
        puts("Interrupt handling test failed.");
    }

    return 0;
}
