
#include <zephyr/kernel.h>
#include <zephyr/irq.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/sys/printk.h>

#define MY_IRQ 15  // Example IRQ number, adjust according to your hardware

static atomic_t interrupt_flag = ATOMIC_INIT(0);

void my_isr(const void *arg)
{
    atomic_set(&interrupt_flag, 1);
    printk("Interrupt occurred!\n");
}

int main(void)
{
    printk("Starting interrupt handling test on ESP32\n");

    // Attach the ISR to the IRQ
    irq_connect_dynamic(MY_IRQ, 0, my_isr, NULL, 0);

    // Enable the IRQ
    irq_enable(MY_IRQ);

    // Test disabling and restoring interrupts
    printk("Disabling interrupts...\n");
    unsigned int key = irq_lock();

    // Simulate an interrupt (this should not trigger the ISR)
    my_isr(NULL);

    // Check if the interrupt flag was set
    if (atomic_get(&interrupt_flag) == 1) {
        printk("Error: Interrupt flag set while interrupts were disabled!\n");
    } else {
        printk("Interrupt flag not set, as expected.\n");
    }

    // Restore interrupts
    irq_unlock(key);

    // Simulate an interrupt (this should trigger the ISR)
    my_isr(NULL);

    // Check if the interrupt flag was set
    if (atomic_get(&interrupt_flag) == 1) {
        printk("Interrupt flag set, as expected.\n");
    } else {
        printk("Error: Interrupt flag not set!\n");
    }

    printk("Interrupt handling test completed.\n");

    return 0;
}
