
#include <zephyr/kernel.h>
#include <zephyr/irq.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/sys/printk.h>

#define IRQ_LINE 1  // Example IRQ line, adjust according to your hardware

static atomic_t interrupt_flag = ATOMIC_INIT(0);

void my_isr(const void *arg)
{
    ARG_UNUSED(arg);

    // Set the interrupt flag
    atomic_set(&interrupt_flag, 1);

    // Clear the interrupt (if necessary)
    // This part depends on the specific hardware and interrupt controller
    // For example, if using a specific peripheral, you might need to clear a status register
}

int main(void)
{
    printk("Starting interrupt handling test on ESP32\n");

    // Register the ISR
    IRQ_CONNECT(IRQ_LINE, 0, my_isr, NULL, 0);

    // Enable the interrupt
    irq_enable(IRQ_LINE);

    // Simulate an interrupt by triggering the IRQ line (this part depends on your hardware)
    // For example, if using a timer or a specific peripheral, you might need to start it here

    // Wait for the interrupt to occur
    while (atomic_get(&interrupt_flag) == 0) {
        k_sleep(K_MSEC(100));
    }

    // Disable interrupts to check if the flag is still set
    irq_disable(IRQ_LINE);

    if (atomic_get(&interrupt_flag) == 1) {
        printk("Interrupt occurred and flag is set correctly\n");
    } else {
        printk("Interrupt did not occur as expected\n");
    }

    // Restore interrupts
    irq_enable(IRQ_LINE);

    printk("Interrupt handling test completed\n");

    return 0;
}
