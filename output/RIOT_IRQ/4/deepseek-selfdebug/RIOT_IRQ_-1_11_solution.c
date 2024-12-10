
#include <stdio.h>
#include <irq.h>
#include <atomic_utils.h>

atomic_t counter = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    atomic_add_u32(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test...\n");

    // Initialize the atomic variable
    atomic_store_u32(&counter, 0);

    // Set the interrupt handler
    irq_set_handler(0, irq_handler, NULL);

    // Trigger the interrupt
    irq_trigger(0);

    // Load the value of the atomic variable
    uint32_t value = atomic_load_u32(&counter);

    printf("Counter value after interrupt: %u\n", value);

    return 0;
}
