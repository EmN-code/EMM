
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define IRQ_LINE (11)  // Assuming IRQ line 11 for this example

static atomic_t interrupt_flag = ATOMIC_INIT(0);

static void irq_handler(void *arg)
{
    (void)arg;
    atomic_set(&interrupt_flag, 1);
    printf("Interrupt received\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Register the interrupt handler
    irq_enable(IRQ_LINE);
    irq_attach(IRQ_LINE, irq_handler, NULL);

    // Disable interrupts
    unsigned state = irq_disable();
    printf("Interrupts disabled\n");

    // Simulate some work with interrupts disabled
    xtimer_sleep(2);

    // Restore interrupts
    irq_restore(state);
    printf("Interrupts restored\n");

    // Wait for an interrupt to be received
    while (atomic_get(&interrupt_flag) == 0) {
        xtimer_usleep(100);
    }

    printf("Interrupt handling test completed\n");

    // Clean up
    irq_disable(IRQ_LINE);
    irq_detach(IRQ_LINE);

    return 0;
}
