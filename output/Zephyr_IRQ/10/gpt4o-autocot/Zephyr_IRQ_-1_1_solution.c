
#include <zephyr/kernel.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/irq.h>
#include <zephyr/init.h>
#include <zephyr/device.h>

#define MY_IRQ 5  // Example IRQ number, adjust according to your hardware

static atomic_t interrupt_flag = ATOMIC_INIT(0);

void my_isr(const void *arg)
{
    ARG_UNUSED(arg);

    // Set the interrupt flag
    atomic_set(&interrupt_flag, 1);

    // Clear the interrupt (if necessary)
    // This part depends on the specific hardware and IRQ
    // For example, if you have a specific register to clear the interrupt:
    // *(volatile uint32_t *)CLEAR_INTERRUPT_REGISTER = 0x1;
}

void test_interrupt_handling(void)
{
    // Disable interrupts
    unsigned int key = irq_lock();

    // Check if interrupts are disabled
    if (irq_is_in_isr()) {
        printk("Interrupts are disabled in ISR context\n");
    } else {
        printk("Interrupts are disabled in thread context\n");
    }

    // Simulate some work
    k_sleep(K_MSEC(100));

    // Restore interrupts
    irq_unlock(key);

    // Check if interrupts are restored
    if (irq_is_in_isr()) {
        printk("Interrupts are still disabled in ISR context\n");
    } else {
        printk("Interrupts are restored in thread context\n");
    }
}

int main(void)
{
    // Configure the interrupt
    IRQ_CONNECT(MY_IRQ, 0, my_isr, NULL, 0);
    irq_enable(MY_IRQ);

    // Test interrupt handling
    test_interrupt_handling();

    // Wait for the interrupt to occur
    while (atomic_get(&interrupt_flag) == 0) {
        k_sleep(K_MSEC(100));
    }

    printk("Interrupt occurred and handled successfully\n");

    return 0;
}
