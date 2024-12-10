
#include "event/thread.h"
#include "irq.h"
#include "thread.h"

#define ISR_FLAG (1 << 0)

static volatile unsigned int isr_flag = 0;

static void isr_handler(void)
{
    isr_flag |= ISR_FLAG;
}

int main(void)
{
    unsigned state;

    // Initialize the ISR flag
    isr_flag = 0;

    // Attach the ISR handler
    irq_attach(UART_IRQ_NUM, isr_handler);

    // Disable interrupts and save the current state
    state = irq_disable();

    // Simulate a critical section
    printf("Entering critical section...\n");
    // Perform some critical operations here

    // Re-enable interrupts
    irq_restore(state);

    // Simulate an interrupt
    printf("Simulating interrupt...\n");
    uart_rxstart_irq_disable(UART_DEV(0));

    // Check if the ISR flag was set
    if (isr_flag & ISR_FLAG) {
        printf("Interrupt was handled correctly.\n");
    } else {
        printf("Interrupt was not handled.\n");
    }

    return 0;
}
