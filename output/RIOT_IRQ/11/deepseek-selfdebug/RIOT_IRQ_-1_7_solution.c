
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int_t atomic_var = ATOMIC_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Set the interrupt handler
    irq_set_handler(interrupt_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify the atomic variable
    atomic_store(&atomic_var, 0);

    // Restore interrupts
    irq_restore(state);

    // Trigger the interrupt (simulated)
    // In a real application, this would be triggered by an external event
    interrupt_handler(NULL);

    // Check the value of the atomic variable
    int value = atomic_load(&atomic_var);
    if (value == 1) {
        printf("Interrupt handling test passed: atomic variable set correctly.\n");
    } else {
        printf("Interrupt handling test failed: atomic variable not set correctly.\n");
    }

    return 0;
}
