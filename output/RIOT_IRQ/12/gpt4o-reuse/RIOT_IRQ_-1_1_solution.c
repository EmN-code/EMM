
#include "irq.h"
#include "thread.h"
#include "xtimer.h"
#include "atomic_utils.h"
#include "periph/cpuid.h"

#define IRQ_NUM         (11)
#define TEST_VALUE      (0x12345678)

static volatile atomic_t test_var = ATOMIC_INIT(TEST_VALUE);

static void irq_handler(void *arg)
{
    (void)arg;
    printk("Interrupt handler triggered\n");
    atomic_store(&test_var, TEST_VALUE + 1);
}

int main(void)
{
    printk("Starting interrupt handling test\n");

    // Register the interrupt handler
    irq_request(IRQ_NUM, irq_handler, NULL);

    // Disable the interrupt
    irq_disable(IRQ_NUM);
    printk("Interrupt disabled\n");

    // Verify the interrupt is disabled by checking the atomic variable
    xtimer_sleep(1);
    if (atomic_load(&test_var) == TEST_VALUE) {
        printk("Interrupt was not triggered as expected\n");
    } else {
        printk("Error: Interrupt was triggered while disabled\n");
    }

    // Enable the interrupt
    irq_enable(IRQ_NUM);
    printk("Interrupt enabled\n");

    // Verify the interrupt is enabled by checking the atomic variable
    xtimer_sleep(1);
    if (atomic_load(&test_var) == TEST_VALUE + 1) {
        printk("Interrupt was triggered as expected\n");
    } else {
        printk("Error: Interrupt was not triggered while enabled\n");
    }

    // Clean up
    irq_disable(IRQ_NUM);
    irq_release(IRQ_NUM);

    printk("Interrupt handling test completed\n");
    return 0;
}
