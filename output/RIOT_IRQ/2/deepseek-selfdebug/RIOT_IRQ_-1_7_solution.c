
#include <stdio.h>
#include <irq_arch.h>
#include <atomic_utils.h>

#define IRQ_NUM 0  // Define the IRQ number to use

atomic_t counter = ATOMIC_INIT(0);  // Initialize atomic variable

void irq_handler(void)
{
    atomic_add(&counter, 1);  // Increment the atomic counter
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Set the IRQ handler
    irq_set_handler(IRQ_NUM, irq_handler);

    // Enable the IRQ
    irq_enable(IRQ_NUM);

    // Simulate some work
    for (int i = 0; i < 1000000; i++) {
        // Do some work
    }

    // Disable the IRQ
    irq_disable(IRQ_NUM);

    // Check the counter value
    printf("Counter value: %d\n", atomic_get(&counter));

    return 0;
}
