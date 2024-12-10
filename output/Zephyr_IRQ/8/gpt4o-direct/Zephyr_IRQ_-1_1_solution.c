
#include <zephyr/kernel.h>
#include <zephyr/irq.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/sys/printk.h>

#define IRQ_LINE 1  // Example IRQ line, adjust according to your hardware

static atomic_t interrupt_flag = ATOMIC_INIT(0);

void custom_isr(const void *arg)
{
    atomic_set(&interrupt_flag, 1);
    printk("Interrupt occurred!\n");
}

int main(void)
{
    printk("Starting interrupt handling test on ESP32\n");

    // Register the custom ISR
    irq_connect_dynamic(IRQ_LINE, 0, custom_isr, NULL, 0);

    // Enable the IRQ line
    irq_enable(IRQ_LINE);

    // Simulate an interrupt by triggering the IRQ line
    // This part depends on how you can trigger the IRQ on your hardware
    // For example, if you have a timer or another peripheral that can generate an interrupt
    // you would configure that here.

    // Wait for the interrupt to occur
    while (atomic_get(&interrupt_flag) == 0) {
        k_sleep(K_MSEC(100));
    }

    // Disable interrupts to check if the flag is still set
    irq_disable(IRQ_LINE);

    if (atomic_get(&interrupt_flag) == 1) {
        printk("Interrupts disabled, flag is still set.\n");
    } else {
        printk("Interrupts disabled, flag was reset.\n");
    }

    // Restore interrupts
    irq_enable(IRQ_LINE);

    printk("Interrupt handling test completed.\n");

    return 0;
}
