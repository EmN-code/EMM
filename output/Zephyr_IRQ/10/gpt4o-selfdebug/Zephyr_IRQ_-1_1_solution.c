#include <zephyr/types.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/sys/printk.h>

/* Atomic variable to determine if interrupt handler
 * executed correctly.
 */
static atomic_t interrupt_handled = ATOMIC_INIT(0);

/* Stub function to simulate an interrupt handler */
void interrupt_handler(void)
{
    /* Disable interrupt using irq_lock */
    unsigned int key = irq_lock();

    /* Simulated work done in interrupt */
    atomic_set(&interrupt_handled, 1);

    /* Restore interrupt state */
    irq_unlock(key);
}

int main(void)
{
    printk("Starting interrupt handling simulation...\n");

    /* Simulate triggering an interrupt */
    interrupt_handler();

    /* Verify if interrupt was handled properly */
    if (atomic_get(&interrupt_handled) == 1) {
        printk("Interrupt handled successfully!\n");
    } else {
        printk("Interrupt handling failed.\n");
    }

    return 0;
}