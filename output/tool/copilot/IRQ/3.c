#include "irq.h"
#include "atomic_utils.h"
#include "xtimer.h"
#include "periph/uart.h"

static volatile uint32_t interrupt_counter = 0;
static volatile uint32_t atomic_counter = 0;

void isr_handler(void *arg)
{
    (void)arg;
    /* Increment the interrupt counter */
    interrupt_counter++;

    /* Perform an atomic increment */
    atomic_fetch_add_u32(&atomic_counter, 1);
}

int main(void)
{
    /* Initialize UART for debugging */
    uart_init(UART_DEV(0), 115200, NULL, NULL);

    /* Print initial values */
    printf("Initial interrupt_counter: %u\n", interrupt_counter);
    printf("Initial atomic_counter: %u\n", atomic_counter);

    /* Disable interrupts */
    uint32_t state = irq_disable();

    /* Set up the interrupt handler */
    irq_enable(IRQ_TIMER0, isr_handler, NULL);

    /* Enable interrupts */
    irq_restore(state);

    /* Simulate an interrupt */
    irq_trigger(IRQ_TIMER0);

    /* Print values after interrupt */
    printf("After interrupt interrupt_counter: %u\n", interrupt_counter);
    printf("After interrupt atomic_counter: %u\n", atomic_counter);

    return 0;
}