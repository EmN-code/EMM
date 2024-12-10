
#include <stdio.h>
#include <stdint.h>
#include <irq.h>
#include <atomic_utils.h>

atomic_int counter = ATOMIC_VAR_INIT(0);

void irq_handler(void *arg)
{
    atomic_fetch_add(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Set the IRQ handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Enable interrupts
    irq_enable();

    // Simulate some work
    for (int i = 0; i < 1000000; i++) {
        // Do some work
    }

    // Disable interrupts
    irq_disable();

    // Check the counter value
    int final_count = atomic_load(&counter);
    printf("Final counter value: %d\n", final_count);

    return 0;
}
