
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic_utils.h>

atomic_int counter = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    (void)arg;
    atomic_add(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Set the interrupt handler
    irq_set_handler(0, irq_handler, NULL);

    // Enable interrupts
    irq_enable();

    // Simulate some work
    for (int i = 0; i < 1000000; i++) {
        // Do some work
    }

    // Disable interrupts
    irq_disable();

    // Check the counter value
    int final_count = atomic_get(&counter);
    printf("Final counter value: %d\n", final_count);

    return 0;
}
