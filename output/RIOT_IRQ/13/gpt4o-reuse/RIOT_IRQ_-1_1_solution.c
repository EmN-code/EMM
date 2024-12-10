
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define IRQ_LINE (11)  // Assuming IRQ line 11 for this example

static volatile atomic_t interrupt_occurred = ATOMIC_INIT(0);

void irq_handler(void)
{
    atomic_set(&interrupt_occurred, 1);
    printf("Interrupt occurred\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Register the IRQ handler
    irq_disable(IRQ_LINE);
    irq_set(IRQ_LINE, irq_handler);
    irq_enable(IRQ_LINE);

    // Disable interrupts and check the atomic variable
    unsigned state = irq_disable();
    if (atomic_get(&interrupt_occurred) == 0) {
        printf("Interrupts disabled correctly\n");
    } else {
        printf("Interrupts were not disabled correctly\n");
    }
    irq_restore(state);

    // Wait for an interrupt to occur
    xtimer_sleep(5);  // Wait for 5 seconds to see if an interrupt occurs

    // Check if the interrupt occurred
    if (atomic_get(&interrupt_occurred) == 1) {
        printf("Interrupt was handled correctly\n");
    } else {
        printf("Interrupt was not handled\n");
    }

    // Cleanup
    irq_disable(IRQ_LINE);
    irq_clear(IRQ_LINE);

    printf("Interrupt handling test completed\n");
    return 0;
}
