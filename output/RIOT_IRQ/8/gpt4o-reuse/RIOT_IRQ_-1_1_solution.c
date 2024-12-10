
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"
#include "periph/cpuid.h"
#include "periph/irq.h"

#define IRQ_LINE (11)  // Assuming IRQ line 11 for this example

static volatile atomic_t interrupt_flag = ATOMIC_INIT(0);

static void irq_handler(void *arg)
{
    (void)arg;
    atomic_store(&interrupt_flag, 1);
    printf("Interrupt received\n");
}

int main(void)
{
    printf("Starting interrupt handling test\n");

    // Register the interrupt handler
    irq_disable(IRQ_LINE);
    irq_set(IRQ_LINE, irq_handler, NULL);
    irq_enable(IRQ_LINE);

    // Test disabling and enabling interrupts
    printf("Disabling interrupt\n");
    irq_disable(IRQ_LINE);
    xtimer_sleep(2);  // Sleep for 2 seconds to simulate no interrupt

    printf("Enabling interrupt\n");
    irq_enable(IRQ_LINE);

    // Wait for an interrupt to be received
    while (atomic_load(&interrupt_flag) == 0) {
        xtimer_usleep(100);  // Small sleep to avoid busy-waiting
    }

    printf("Interrupt handled successfully\n");

    // Clean up
    irq_disable(IRQ_LINE);
    irq_clear(IRQ_LINE);

    return 0;
}
