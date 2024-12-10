
#include <zephyr/kernel.h>
#include <zephyr/irq.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/sys/printk.h>

#define IRQ_LINE 1  // Example IRQ line, adjust according to your hardware

static atomic_t interrupt_flag = ATOMIC_INIT(0);

void irq_handler(const void *arg)
{
    atomic_set(&interrupt_flag, 1);
    printk("Interrupt handled!\n");
}

int main(void)
{
    printk("Starting interrupt handling test on ESP32\n");

    // Register the IRQ handler
    irq_connect_dynamic(IRQ_LINE, 0, irq_handler, NULL, 0);

    // Enable the IRQ line
    irq_enable(IRQ_LINE);

    // Simulate an interrupt by triggering the IRQ line
    // This part depends on how your hardware can trigger the IRQ line
    // For example, if you have a timer or another peripheral that can trigger the IRQ
    // you would configure that here.

    // Wait for the interrupt to be handled
    while (atomic_get(&interrupt_flag) == 0) {
        k_sleep(K_MSEC(100));
    }

    // Disable interrupts to test atomic operations
    irq_disable(IRQ_LINE);
    printk("Interrupts disabled\n");

    // Perform some atomic operations
    atomic_set(&interrupt_flag, 0);
    printk("Atomic variable reset\n");

    // Restore interrupts
    irq_enable(IRQ_LINE);
    printk("Interrupts restored\n");

    // Wait for another interrupt to be handled
    while (atomic_get(&interrupt_flag) == 0) {
        k_sleep(K_MSEC(100));
    }

    printk("Interrupt handled again after restoring interrupts\n");

    return 0;
}
