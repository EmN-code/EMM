#include "irq.h"
#include "atomic_utils.h"
#include "xtimer.h"
#include "periph/irq.h"

static volatile uint32_t irq_counter = 0;
static volatile uint32_t atomic_counter = 0;

void irq_handler(void *arg)
{
    (void)arg;
    // Increment the IRQ counter atomically
    atomic_fetch_add_u32(&irq_counter, 1);
    // Perform some operations
    atomic_fetch_add_u32(&atomic_counter, 1);
}

int main(void)
{
    // Initialize the IRQ
    irq_disable();
    irq_enable();

    // Set up the IRQ handler
    irq_set(IRQ_TIMER0, irq_handler, NULL);

    // Enable the IRQ
    irq_enable();

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);
        // Disable interrupts
        irq_disable();
        // Perform some operations
        atomic_fetch_add_u32(&atomic_counter, 1);
        // Enable interrupts
        irq_enable();
    }

    // Print the results
    printf("IRQ counter: %lu\n", irq_counter);
    printf("Atomic counter: %lu\n", atomic_counter);

    return 0;
}