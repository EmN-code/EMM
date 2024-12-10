
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

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
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify the atomic variable while interrupts are disabled
    atomic_store(&atomic_var, 0);

    // Restore interrupts
    irq_restore(state);

    // Trigger the interrupt
    irq_trigger(IRQ_NUM);

    // Wait for a short period to allow the interrupt to be handled
    thread_sleep(100000);

    // Check the value of the atomic variable
    int value = atomic_load(&atomic_var);
    if (value == 1) {
        puts("Interrupt was handled correctly.");
    } else {
        puts("Interrupt handling failed.");
    }

    return 0;
}
