
#include <stdio.h>
#include <stdint.h>
#include <irq.h>
#include <atomic.h>

atomic_int counter = ATOMIC_INIT(0);

void irq_handler(void *arg)
{
    atomic_fetch_add(&counter, 1);
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Set the IRQ handler
    irq_set_handler(0, irq_handler, NULL);

    // Trigger the IRQ
    irq_trigger(0);

    // Wait for the interrupt to be handled
    xtimer_sleep(1);

    // Check the counter value
    int final_count = atomic_load(&counter);
    printf("Counter value after interrupt: %d\n", final_count);

    return 0;
}
