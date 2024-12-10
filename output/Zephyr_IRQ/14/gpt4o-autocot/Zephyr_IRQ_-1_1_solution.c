
#include <zephyr/kernel.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/irq.h>
#include <zephyr/init.h>
#include <zephyr/device.h>

#define MY_IRQ 15  // Example IRQ number, adjust according to your hardware

static atomic_t interrupt_flag = ATOMIC_INIT(0);

void my_isr(const void *arg)
{
    atomic_set(&interrupt_flag, 1);
    printk("Interrupt handled!\n");
}

int main(void)
{
    unsigned int key;

    // Register the ISR for the specific IRQ
    irq_connect_dynamic(MY_IRQ, 0, my_isr, NULL, 0);

    // Enable the IRQ
    irq_enable(MY_IRQ);

    // Test disabling and restoring interrupts
    key = irq_lock();
    printk("Interrupts disabled, generating an interrupt...\n");
    // Simulate an interrupt by triggering the IRQ (this is hardware-specific)
    // For example, on some platforms, you might write to a specific register
    // to trigger the IRQ.
    // Trigger the IRQ here
    k_sleep(K_MSEC(100));  // Wait for the interrupt to be handled
    if (atomic_get(&interrupt_flag) == 1) {
        printk("Interrupt was handled while interrupts were disabled!\n");
    } else {
        printk("Interrupt was not handled while interrupts were disabled.\n");
    }
    irq_unlock(key);

    // Reset the interrupt flag
    atomic_set(&interrupt_flag, 0);

    // Test enabling interrupts
    printk("Interrupts enabled, generating an interrupt...\n");
    // Trigger the IRQ here
    k_sleep(K_MSEC(100));  // Wait for the interrupt to be handled
    if (atomic_get(&interrupt_flag) == 1) {
        printk("Interrupt was handled while interrupts were enabled!\n");
    } else {
        printk("Interrupt was not handled while interrupts were enabled.\n");
    }

    return 0;
}
