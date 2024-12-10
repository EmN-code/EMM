
#include <stdio.h>
#include <stdint.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int counter = ATOMIC_VAR_INIT(0);

void interrupt_handler(void *arg)
{
    (void)arg;
    atomic_store(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Set the interrupt handler
    irq_set_handler(interrupt_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some critical section code
    printf("Critical section: counter = %d\n", atomic_load(&counter));

    // Restore interrupts
    irq_restore(state);

    // Trigger the interrupt
    irq_enable();

    // Wait for the interrupt to be handled
    thread_sleep();

    // Check the counter value after interrupt
    printf("After interrupt: counter = %d\n", atomic_load(&counter));

    return 0;
}
