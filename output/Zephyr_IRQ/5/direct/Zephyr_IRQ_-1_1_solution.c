
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

    // Acknowledge the interrupt (if necessary)
    // This part depends on the specific hardware and IRQ line
    // For example, if using a specific peripheral, you might need to clear a status register
}

int main(void)
{
    printk("Interrupt Handling Example\n");

    // Configure the IRQ line
    IRQ_CONNECT(IRQ_LINE, 0, irq_handler, NULL, 0);

    // Enable the IRQ line
    irq_enable(IRQ_LINE);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        // Disable interrupts
        unsigned int key = irq_lock();

        // Check if the interrupt flag was set while interrupts were disabled
        if (atomic_get(&interrupt_flag) == 1) {
            printk("Interrupt occurred while interrupts were disabled\n");
        } else {
            printk("No interrupt occurred while interrupts were disabled\n");
        }

        // Restore interrupts
        irq_unlock(key);

        // Clear the interrupt flag
        atomic_set(&interrupt_flag, 0);

        // Simulate a delay to allow the interrupt to occur
        k_sleep(K_MSEC(100));
    }

    printk("End of Interrupt Handling Example\n");
    return 0;
}
