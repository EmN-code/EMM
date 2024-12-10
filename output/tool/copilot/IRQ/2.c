#include <stdio.h>
#include "irq.h"
#include "atomic_utils.h"
#include "xtimer.h"

static volatile uint32_t interrupt_counter = 0;
static volatile uint32_t main_counter = 0;

void isr_handler(void *arg)
{
    (void)arg;
    // Increment the interrupt counter atomically
    atomic_fetch_add_u32(&interrupt_counter, 1);
}

int main(void)
{
    // Initialize the interrupt
    irq_disable();
    irq_set(IRQ_TIMER0, isr_handler, NULL);
    irq_enable(IRQ_TIMER0);
    irq_restore();

    // Main loop
    while (1) {
        // Disable interrupts
        uint32_t state = irq_disable();

        // Increment the main counter atomically
        atomic_fetch_add_u32(&main_counter, 1);

        // Restore interrupts
        irq_restore(state);

        // Print the counters
        printf("Main counter: %u, Interrupt counter: %u\n", main_counter, interrupt_counter);

        // Sleep for a while
        xtimer_sleep(1);
    }

    return 0;
}