#include "irq.h"
#include "atomic_utils.h"
#include "xtimer.h"
#include "thread.h"
#include "periph/uart.h"

static volatile uint32_t irq_counter = 0;
static volatile uint32_t atomic_counter = 0;

void irq_handler(void *arg)
{
    (void)arg;
    atomic_fetch_add_u32(&atomic_counter, 1);
    irq_counter++;
}

int main(void)
{
    // Disable interrupts
    uint32_t state = irq_disable();

    // Set up the interrupt handler
    irq_enable(IRQ_TIMER0);
    irq_set(IRQ_TIMER0, irq_handler, NULL);

    // Enable interrupts
    irq_restore(state);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        xtimer_sleep(1);
        printf("Atomic counter: %lu, IRQ counter: %lu\n", atomic_counter, irq_counter);
    }

    return 0;
}