
#include <stdio.h>
#include <irq.h>
#include <thread.h>
#include <atomic.h>

atomic_t counter = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    atomic_add(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Set the interrupt handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Enable the interrupt
    irq_enable(IRQ_NUM);

    // Simulate some work
    for (int i = 0; i < 1000000; i++) {
        // Do some work
    }

    // Disable the interrupt
    irq_disable(IRQ_NUM);

    // Check the counter value
    printf("Counter value: %d\n", atomic_get(&counter));

    return 0;
}
