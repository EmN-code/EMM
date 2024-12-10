
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
    printf("Starting interrupt handling test...\n");

    // Set the interrupt handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Modify the atomic variable
    atomic_store(&atomic_var, 0);

    // Restore interrupts
    irq_restore(state);

    // Trigger the interrupt
    irq_trigger(IRQ_NUM);

    // Wait for the interrupt to be handled
    thread_sleep(100000);

    // Check the atomic variable
    int value = atomic_load(&atomic_var);
    if (value == 1) {
        printf("Interrupt handling test passed!\n");
    } else {
        printf("Interrupt handling test failed!\n");
    }

    return 0;
}
