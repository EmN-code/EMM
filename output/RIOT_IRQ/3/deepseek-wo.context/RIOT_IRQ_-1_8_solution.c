
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_t flag = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&flag, 1);
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Initialize the atomic variable
    atomic_store(&flag, 0);

    // Set the interrupt handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Perform some critical section code
    printf("Critical section: flag = %d\n", atomic_load(&flag));

    // Restore interrupts
    irq_restore(state);

    // Wait for the interrupt to be handled
    thread_sleep();

    // Check the flag after interrupt
    printf("After interrupt: flag = %d\n", atomic_load(&flag));

    return 0;
}
