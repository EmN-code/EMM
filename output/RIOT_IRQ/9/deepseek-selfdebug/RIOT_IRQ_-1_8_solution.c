
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_t flag = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&flag, 1);
}

int main(void)
{
    puts("Starting interrupt handling test...");

    // Set the IRQ handler
    irq_set_handler(0, irq_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify the atomic variable
    atomic_store(&flag, 0);

    // Restore interrupts
    irq_restore(state);

    // Trigger the interrupt
    irq_set(0);

    // Wait for the interrupt to be handled
    thread_sleep();

    // Check the atomic variable
    if (atomic_load(&flag) == 1) {
        puts("Interrupt handling test passed!");
    } else {
        puts("Interrupt handling test failed!");
    }

    return 0;
}
