#include "xtimer.h"
#include "irq_arch.h"
#include "assert.h"
#include "stdbool.h"

volatile uint32_t counter = 0;
volatile bool irq_disabled = false;

void timer_callback(void)
{
    // Increment the counter safely with interrupts disabled
    unsigned state = irq_disable();
    counter++;
    irq_restore(state);
}

int main(void)
{
    // Set up the timer
    xtimer_ticks32_t now = xtimer_ticks64(); // Correct function
    unsigned state;
    
    // Disable interrupts
    state = irq_disable();
    irq_disabled = true;

    // Do some work with interrupts disabled
    counter = 100;

    // Check irq_disabled status
    assert(irq_disabled == true);

    // Restore interrupts
    irq_restore(state);
    assert(irq_disabled == false);

    // Continue with main operations
    while (true)
    {
        // Simulate periodic operations
        if (xtimer_usec_from_ticks(xtimer_diff(xtimer_ticks64(), now)) > 1000000) {
            timer_callback();
            now = xtimer_ticks64();
        }
    }
    return 0;
}