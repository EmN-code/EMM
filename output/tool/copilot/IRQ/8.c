#include "irq.h"
#include "atomic_utils.h"
#include "xtimer.h"
#include "thread.h"
#include "periph/uart.h"
static volatile uint32_t atomic_counter = 0;
static volatile bool irq_status = false;
void isr_handler(void *arg)
{
    (void)arg;
    // Increment the atomic counter
    atomic_fetch_add_u32(&atomic_counter, 1);
    // Set the interrupt status flag
    irq_status = true;
}
void init_interrupt(void)
{
    // Disable interrupts
    uint32_t state = irq_disable();

    // Set the interrupt handler
    irq_set(IRQ_TIMER0, isr_handler, NULL);

    // Enable the interrupt
    irq_enable(IRQ_TIMER0);

    // Restore interrupts
    irq_restore(state);
}
int main(void)
{
    // Initialize the interrupt
    init_interrupt();

    // Simulate an interrupt
    irq_handler(NULL);

    // Check the atomic variable and interrupt status
    if (atomic_load_u32(&atomic_counter) > 0 && irq_status) {
        printf("Interrupt handled successfully\n");
    } else {
        printf("Interrupt handling failed\n");
    }

    return 0;
}