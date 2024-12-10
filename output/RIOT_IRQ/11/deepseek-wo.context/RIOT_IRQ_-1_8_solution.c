
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_types.h>

atomic_int_t atomic_var = ATOMIC_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    puts("Starting interrupt handling test...");

    // Set the interrupt handler
    irq_set_handler(0, interrupt_handler, NULL);

    // Trigger the interrupt
    irq_trigger(0);

    // Wait for the interrupt to be handled
    while (atomic_load(&atomic_var) == 0) {
        thread_yield();
    }

    puts("Interrupt handled successfully.");

    return 0;
}
