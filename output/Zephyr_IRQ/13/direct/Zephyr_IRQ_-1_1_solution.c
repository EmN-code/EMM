
#include <zephyr/kernel.h>
#include <zephyr/irq.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/sys/printk.h>

#define IRQ_LINE 1  // Example IRQ line, adjust according to your hardware

volatile atomic_t interrupt_occurred = ATOMIC_INIT(0);

void my_isr(const void *arg)
{
    ARG_UNUSED(arg);

    // Set the atomic variable to indicate an interrupt occurred
    atomic_set(&interrupt_occurred, 1);

    // Acknowledge the interrupt (if necessary)
    // This part depends on the specific hardware and IRQ line
    // For example, if using a specific peripheral, you might need to clear a flag
    // or write to a register to acknowledge the interrupt.
}

int main(void)
{
    printk("Starting interrupt handling test on ESP32\n");

    // Register the ISR for the IRQ line
    irq_connect_dynamic(IRQ_LINE, 0, my_isr, NULL, 0);

    // Enable the IRQ line
    irq_enable(IRQ_LINE);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        // Disable interrupts
        unsigned int key = irq_lock();

        // Check if an interrupt occurred while interrupts were disabled
        if (atomic_get(&interrupt_occurred) == 1) {
            printk("Interrupt occurred while interrupts were disabled\n");
        } else {
            printk("No interrupt occurred while interrupts were disabled\n");
        }

        // Restore interrupts
        irq_unlock(key);

        // Wait for a short period to allow interrupts to occur
        k_sleep(K_MSEC(100));
    }

    // Disable the IRQ line
    irq_disable(IRQ_LINE);

    printk("Interrupt handling test completed\n");

    return 0;
}
