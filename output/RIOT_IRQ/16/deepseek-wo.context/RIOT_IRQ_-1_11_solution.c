
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define IRQ_NUM 0  // Define the IRQ number to use

volatile atomic_t counter = ATOMIC_INIT(0);  // Initialize atomic variable

void irq_handler(void *arg)
{
    (void)arg;  // Unused parameter
    atomic_inc(&counter);  // Increment the atomic counter
}

int main(void)
{
    // Set the IRQ handler
    irq_set_handler(IRQ_NUM, irq_handler, NULL);

    // Enable the IRQ
    irq_enable(IRQ_NUM);

    // Simulate some work
    xtimer_sleep(1);

    // Disable the IRQ
    irq_disable(IRQ_NUM);

    // Get the value of the atomic counter
    int final_count = atomic_get(&counter);

    // Print the final count
    printf("Final count: %d\n", final_count);

    return 0;
}
