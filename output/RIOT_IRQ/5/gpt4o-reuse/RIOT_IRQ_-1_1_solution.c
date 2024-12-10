
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"

#define IRQ_LINE (11)  // Assuming IRQ line 11 for this example

static volatile atomic_t interrupt_flag = ATOMIC_INIT(0);

void irq_handler(void)
{
    atomic_set(&interrupt_flag, 1);
    printf("Interrupt received\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Register the IRQ handler
    irq_enable(IRQ_LINE);
    irq_set(IRQ_LINE, (irq_handler_t)irq_handler);

    // Disable interrupts and check the flag
    unsigned state = irq_disable();
    printf("Interrupts disabled\n");

    // Simulate some work
    xtimer_sleep(2);

    // Check if the interrupt was received (should be 0 since interrupts are disabled)
    if (atomic_get(&interrupt_flag) == 0) {
        printf("Interrupt was not received while interrupts were disabled\n");
    } else {
        printf("Interrupt was received while interrupts were disabled (unexpected)\n");
    }

    // Restore interrupts
    irq_restore(state);
    printf("Interrupts restored\n");

    // Simulate some work
    xtimer_sleep(2);

    // Check if the interrupt was received (should be 1 since interrupts are enabled)
    if (atomic_get(&interrupt_flag) == 1) {
        printf("Interrupt was received after interrupts were restored\n");
    } else {
        printf("Interrupt was not received after interrupts were restored (unexpected)\n");
    }

    // Unregister the IRQ handler
    irq_disable(IRQ_LINE);
    irq_clear(IRQ_LINE);

    printf("Interrupt handling test completed\n");

    return 0;
}
