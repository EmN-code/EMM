
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int counter = ATOMIC_VAR_INIT(0);

void irq_handler(void *arg)
{
    (void)arg;
    atomic_fetch_add(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Set the interrupt handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Restore interrupts
    irq_restore(state);

    // Trigger the interrupt
    irq_trigger(IRQ_NUM);

    // Wait for the interrupt to be handled
    thread_sleep();

    // Check the counter value
    int final_count = atomic_load(&counter);
    printf("Counter value after interrupt: %d\n", final_count);

    return 0;
}
