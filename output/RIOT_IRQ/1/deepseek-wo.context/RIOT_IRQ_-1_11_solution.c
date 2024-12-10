
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_types.h>

atomic_int counter = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    atomic_add(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Set the interrupt handler
    irq_set_handler(0, irq_handler, NULL);

    // Trigger the interrupt
    irq_trigger(0);

    // Sleep for a while to allow the interrupt to be handled
    thread_sleep();

    // Check the counter value
    int final_count = atomic_get(&counter);
    printf("Counter value after interrupt: %d\n", final_count);

    return 0;
}
