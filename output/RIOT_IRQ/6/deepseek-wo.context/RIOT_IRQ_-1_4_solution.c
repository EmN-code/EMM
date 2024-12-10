
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int counter = ATOMIC_VAR_INIT(0);

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&counter, 1);
}

int main(void)
{
    puts("Interrupt handling test");

    // Set the interrupt handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Trigger the interrupt
    irq_trigger(IRQ_NUM);

    // Wait for the interrupt to be handled
    while (atomic_load(&counter) == 0) {
        thread_yield();
    }

    puts("Interrupt handled successfully");

    return 0;
}
