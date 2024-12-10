
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int counter = ATOMIC_VAR_INIT(0);

void increment_counter(void)
{
    atomic_fetch_add(&counter, 1);
}

void interrupt_handler(void)
{
    printf("Interrupt occurred, counter value: %d\n", atomic_load(&counter));
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Disable interrupts
    unsigned state = irq_disable();

    // Increment counter in an atomic manner
    increment_counter();

    // Restore interrupts
    irq_restore(state);

    // Simulate an interrupt
    interrupt_handler();

    return 0;
}
