#include "irq.h"
#include "atomic_utils.h"
#include "xtimer.h"
#include "thread.h"
#include "periph/irq.h"

static volatile uint32_t atomic_counter = 0;

void isr_handler(void *arg)
{
    (void)arg;
    atomic_fetch_add_u32(&atomic_counter, 1);
}

int main(void)
{
    // Disable interrupts
    uint32_t state = irq_disable();

    // Set up the interrupt handler
    irq_enable(IRQ_TIMER0);
    irq_set(IRQ_TIMER0, isr_handler, NULL);

    // Enable interrupts
    irq_restore(state);

    // Simulate an interrupt
    irq_trigger(IRQ_TIMER0);

    // Wait for a short period to ensure the interrupt is handled
    xtimer_sleep(1);

    // Check the atomic counter value
    if (atomic_load_u32(&atomic_counter) == 1) {
        printf("Interrupt handling and atomic operations work correctly.\n");
    } else {
        printf("There was an issue with interrupt handling or atomic operations.\n");
    }

    return 0;
}