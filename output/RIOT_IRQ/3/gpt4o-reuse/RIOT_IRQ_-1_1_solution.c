
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"
#include "periph/irq.h"

#define IRQ_LINE (11)  // Assuming IRQ line 11 for this example

static volatile atomic_t irq_flag = ATOMIC_INIT(0);

static void irq_handler(void *arg)
{
    (void)arg;
    atomic_set(&irq_flag, 1);
    printf("Interrupt occurred\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Register the interrupt handler
    irq_request(IRQ_LINE, IRQ_TRIGGER_FALLING, irq_handler, NULL);

    // Disable the interrupt
    irq_disable(IRQ_LINE);
    printf("Interrupt disabled\n");

    // Simulate some work
    xtimer_sleep(2);

    // Check if the interrupt flag was set (should not be set)
    if (atomic_get(&irq_flag) == 1) {
        printf("Error: Interrupt flag set while disabled\n");
    } else {
        printf("Interrupt flag not set as expected\n");
    }

    // Enable the interrupt
    irq_enable(IRQ_LINE);
    printf("Interrupt enabled\n");

    // Simulate some work
    xtimer_sleep(2);

    // Check if the interrupt flag was set (should be set)
    if (atomic_get(&irq_flag) == 1) {
        printf("Interrupt flag set as expected\n");
    } else {
        printf("Error: Interrupt flag not set\n");
    }

    // Unregister the interrupt handler
    irq_disable(IRQ_LINE);
    irq_release(IRQ_LINE);

    printf("Interrupt handling test completed\n");

    return 0;
}
