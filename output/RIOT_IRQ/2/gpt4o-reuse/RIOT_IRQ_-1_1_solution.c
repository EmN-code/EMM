
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"
#include "periph/irq.h"

#define IRQ_LINE (11)  // Assuming IRQ line 11 for this example

static atomic_t irq_flag = ATOMIC_INIT(0);

static void irq_handler(void *arg)
{
    (void)arg;
    atomic_set(&irq_flag, 1);
    printf("Interrupt received\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Register the interrupt handler
    irq_request(IRQ_LINE, irq_handler, NULL);

    // Disable the interrupt
    irq_disable(IRQ_LINE);
    printf("Interrupt disabled\n");

    // Simulate some work
    xtimer_sleep(2);

    // Check if the interrupt was received (should not be)
    if (atomic_get(&irq_flag) == 1) {
        printf("Error: Interrupt received while disabled\n");
    } else {
        printf("Interrupt not received as expected\n");
    }

    // Enable the interrupt
    irq_enable(IRQ_LINE);
    printf("Interrupt enabled\n");

    // Simulate some work
    xtimer_sleep(2);

    // Check if the interrupt was received (should be)
    if (atomic_get(&irq_flag) == 1) {
        printf("Interrupt received as expected\n");
    } else {
        printf("Error: Interrupt not received\n");
    }

    // Cleanup
    irq_disable(IRQ_LINE);
    irq_release(IRQ_LINE);

    printf("Interrupt handling test completed\n");

    return 0;
}
