
#include <zephyr/kernel.h>
#include <zephyr/irq.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/sys/printk.h>

#define IRQ_LINE 1  // Example IRQ line, adjust according to your hardware

static atomic_t interrupt_flag = ATOMIC_INIT(0);

void irq_handler(const void *arg)
{
    ARG_UNUSED(arg);

    // Set the interrupt flag
    atomic_set(&interrupt_flag, 1);

    // Clear the interrupt (if necessary)
    // This part depends on the specific hardware and IRQ line
    // For example, if this is a timer interrupt, you might need to clear the timer interrupt flag
}

int main(void)
{
    printk("Interrupt Handling Example\n");

    // Register the IRQ handler
    irq_connect_dynamic(IRQ_LINE, 0, irq_handler, NULL, 0);

    // Enable the IRQ line
    irq_enable(IRQ_LINE);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        // Disable interrupts
        irq_disable(IRQ_LINE);

        // Check if the interrupt flag was set while interrupts were disabled
        if (atomic_get(&interrupt_flag) == 1) {
            printk("Interrupt occurred while interrupts were disabled!\n");
        } else {
            printk("No interrupt occurred while interrupts were disabled.\n");
        }

        // Restore interrupts
        irq_enable(IRQ_LINE);

        // Wait for an interrupt to occur
        k_sleep(K_MSEC(100));

        // Check the interrupt flag
        if (atomic_get(&interrupt_flag) == 1) {
            printk("Interrupt occurred.\n");
            atomic_set(&interrupt_flag, 0);  // Clear the flag
        } else {
            printk("No interrupt occurred.\n");
        }
    }

    return 0;
}
