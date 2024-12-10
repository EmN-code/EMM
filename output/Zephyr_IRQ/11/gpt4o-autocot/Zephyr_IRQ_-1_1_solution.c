
#include <zephyr/kernel.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/irq.h>
#include <zephyr/init.h>

#define MY_IRQ 15  // Example IRQ number for ESP32

static atomic_t interrupt_flag = ATOMIC_INIT(0);

void my_isr(const void *arg)
{
    ARG_UNUSED(arg);

    // Set the interrupt flag
    atomic_set(&interrupt_flag, 1);
}

void test_interrupt_handling(void)
{
    // Disable interrupts
    unsigned int key = irq_lock();

    // Check if interrupts are disabled
    if (irq_is_in_isr()) {
        printk("Interrupts are disabled in ISR context.\n");
    } else {
        printk("Interrupts are disabled in thread context.\n");
    }

    // Restore interrupts
    irq_unlock(key);

    // Check if interrupts are restored
    if (irq_is_in_isr()) {
        printk("Interrupts are still in ISR context.\n");
    } else {
        printk("Interrupts are restored to thread context.\n");
    }
}

void main(void)
{
    // Attach the ISR to the IRQ
    irq_connect_dynamic(MY_IRQ, 0, my_isr, NULL, 0);

    // Enable the IRQ
    irq_enable(MY_IRQ);

    // Test interrupt handling
    test_interrupt_handling();

    // Wait for the interrupt to occur
    while (atomic_get(&interrupt_flag) == 0) {
        k_sleep(K_MSEC(100));
    }

    printk("Interrupt occurred and flag was set.\n");
}
