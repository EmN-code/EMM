
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

    // Clear the interrupt (if necessary)
    // This part depends on the specific hardware and how the interrupt is cleared
    // For example, if the interrupt is edge-triggered, you might need to clear the source.
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

    // Perform some operations that should not be interrupted
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
    // Register the ISR for the specific IRQ
    IRQ_CONNECT(MY_IRQ, 0, my_isr, NULL, 0);

    // Enable the IRQ
    irq_enable(MY_IRQ);

    // Test the interrupt handling
    test_interrupt_handling();

    // Wait for the interrupt to occur
    while (atomic_get(&interrupt_flag) == 0) {
        k_sleep(K_MSEC(100));
    }

    printk("Interrupt occurred and handled successfully\n");

    return 0;
}
