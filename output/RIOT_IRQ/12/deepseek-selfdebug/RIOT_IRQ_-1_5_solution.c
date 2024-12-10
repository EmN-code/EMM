
#include <stdio.h>
#include <thread.h>
#include <irq.h>
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

    // Set the IRQ handler
    irq_set_handler(0, irq_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();

    // Increment the counter in an atomic way
    atomic_fetch_add(&counter, 1);

    // Restore interrupts
    irq_restore(state);

    // Sleep for a while to allow interrupts to occur
    thread_sleep();

    // Check the counter value
    int final_count = atomic_load(&counter);
    printf("Final counter value: %d\n", final_count);

    return 0;
}
