
#include <zephyr/kernel.h>
#include <zephyr/irq.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/sys/printk.h>

#define MY_IRQ 15  // Example IRQ number, adjust according to your hardware

volatile atomic_t interrupt_occurred = ATOMIC_INIT(0);

void my_isr(const void *arg)
{
    ARG_UNUSED(arg);

    // Set the atomic variable to indicate an interrupt occurred
    atomic_set(&interrupt_occurred, 1);

    // Acknowledge the interrupt (if necessary)
    // This part depends on the specific hardware and IRQ controller
    // For example, if using NVIC on ARM, you might do something like:
    // NVIC_ClearPendingIRQ(MY_IRQ);
}

int main(void)
{
    printk("Starting interrupt handling test on ESP32\n");

    // Register the ISR for the specific IRQ
    irq_connect_dynamic(MY_IRQ, 0, my_isr, NULL, 0);

    // Enable the IRQ
    irq_enable(MY_IRQ);

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

    // Disable the IRQ
    irq_disable(MY_IRQ);

    printk("Interrupt handling test completed\n");

    return 0;
}
