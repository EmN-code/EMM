
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int_t atomic_var = ATOMIC_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    printf("Interrupt triggered!\n");
    atomic_store(&atomic_var, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Set up the interrupt handler
    irq_set_handler(IRQ_NUM, interrupt_handler, NULL);

    // Enable interrupts
    irq_restore(state);

    // Simulate an interrupt by manually triggering it
    irq_trigger(IRQ_NUM);

    // Wait for the interrupt to be handled
    while (atomic_load(&atomic_var) == 0) {
        thread_yield();
    }

    printf("Interrupt handling test completed.\n");

    return 0;
}
