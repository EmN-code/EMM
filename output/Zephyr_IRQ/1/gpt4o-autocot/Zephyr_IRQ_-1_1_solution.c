
#include <zephyr/kernel.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/irq.h>
#include <zephyr/init.h>

#define MY_IRQ 15  // Example IRQ number, adjust according to your ESP32 configuration

static atomic_t interrupt_flag = ATOMIC_INIT(0);

void my_isr(const void *arg)
{
    ARG_UNUSED(arg);

    // Set the interrupt flag
    atomic_set(&interrupt_flag, 1);

    // Clear the interrupt (if necessary)
    // This part depends on the specific hardware and how the interrupt is triggered
    // For example, if it's a timer interrupt, you might need to clear the timer flag
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
    irq_connect_dynamic(MY_IRQ, 0, my_isr, NULL, 0);

    // Enable the IRQ
    irq_enable(MY_IRQ);

    // Test the interrupt handling
    test_interrupt_handling();

    // Main loop
    while (1) {
        if (atomic_get(&interrupt_flag)) {
            printk("Interrupt occurred!\n");
            atomic_set(&interrupt_flag, 0);  // Clear the flag
        }
        k_sleep(K_SECONDS(1));
    }

    return 0;
}
