
#include <zephyr/kernel.h>
#include <zephyr/irq.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/sys/printk.h>

#define MY_IRQ 5  // Example IRQ number, adjust according to your hardware

static atomic_t interrupt_flag = ATOMIC_INIT(0);

void my_isr(const void *arg)
{
    atomic_set(&interrupt_flag, 1);
    printk("Interrupt occurred!\n");
}

int main(void)
{
    printk("Starting interrupt handling test on ESP32\n");

    // Register the ISR for the specified IRQ
    irq_connect_dynamic(MY_IRQ, 0, my_isr, NULL, 0);

    // Enable the IRQ
    irq_enable(MY_IRQ);

    // Test disabling and restoring interrupts
    printk("Disabling interrupts...\n");
    unsigned int key = irq_lock();

    // Simulate some work while interrupts are disabled
    k_sleep(K_MSEC(100));

    // Check if an interrupt occurred (should not happen)
    if (atomic_get(&interrupt_flag) == 1) {
        printk("Error: Interrupt occurred while interrupts were disabled!\n");
    } else {
        printk("No interrupt occurred while interrupts were disabled.\n");
    }

    // Restore interrupts
    irq_unlock(key);
    printk("Interrupts restored.\n");

    // Wait for an interrupt to occur
    k_sleep(K_SECONDS(1));

    // Check if an interrupt occurred (should happen)
    if (atomic_get(&interrupt_flag) == 1) {
        printk("Interrupt successfully occurred after restoring interrupts.\n");
    } else {
        printk("Error: Interrupt did not occur after restoring interrupts!\n");
    }

    return 0;
}