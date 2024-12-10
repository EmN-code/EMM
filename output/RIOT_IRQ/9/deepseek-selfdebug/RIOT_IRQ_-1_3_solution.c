
#include <stdio.h>
#include <stdint.h>
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

    // Register an interrupt handler
    irq_disable();
    irq_set_handler(0, interrupt_handler, NULL);
    irq_enable();

    // Trigger the interrupt
    irq_trigger(0);

    // Wait for the interrupt to be handled
    thread_sleep();

    // Check the atomic variable
    if (atomic_load(&atomic_var) == 1) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt handling failed.\n");
    }

    return 0;
}
