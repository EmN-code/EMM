
#include <zephyr/kernel.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/irq.h>
#include <zephyr/init.h>
#include <zephyr/device.h>

#define MY_IRQ 15  // Example IRQ number, adjust according to your ESP32 configuration

static atomic_t interrupt_flag = ATOMIC_INIT(0);

void my_isr(const void *arg)
{
    ARG_UNUSED(arg);

    // Set the interrupt flag
    atomic_set(&interrupt_flag, 1);

    // Clear the interrupt (if necessary)
    // This part depends on the specific hardware and how the interrupt is triggered and cleared
    // For example, if it's a timer interrupt, you might need to reset the timer here.
}

void test_interrupt_handling(void)
{
    // Disable interrupts
    unsigned int key = irq_lock();

    // Check if the interrupt flag is set (it shouldn't be)
    if (atomic_get(&interrupt_flag) != 0) {
        printk("Interrupt flag was set while interrupts were disabled!\n");
    } else {
        printk("Interrupt flag is not set, as expected.\n");
    }

    // Restore interrupts
    irq_unlock(key);

    // Wait for the interrupt to be triggered
    k_sleep(K_MSEC(100));  // Adjust sleep duration as needed

    // Check if the interrupt flag is set (it should be)
    if (atomic_get(&interrupt_flag) != 1) {
        printk("Interrupt flag was not set after restoring interrupts!\n");
    } else {
        printk("Interrupt flag was set, as expected.\n");
    }
}

int main(void)
{
    // Configure the interrupt
    IRQ_CONNECT(MY_IRQ, 0, my_isr, NULL, 0);

    // Enable the interrupt
    irq_enable(MY_IRQ);

    // Test the interrupt handling
    test_interrupt_handling();

    return 0;
}
