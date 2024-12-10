
#include <zephyr/kernel.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/irq.h>
#include <zephyr/init.h>
#include <zephyr/device.h>

#define MY_IRQ 15  // Example IRQ number, adjust according to your hardware

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

    // Check if the interrupt flag is set
    if (atomic_get(&interrupt_flag) == 1) {
        printk("Interrupt was handled while interrupts were disabled!\n");
    } else {
        printk("Interrupt was not handled while interrupts were disabled.\n");
    }

    // Restore interrupts
    irq_unlock(key);
}

void main(void)
{
    // Register the ISR for the specific IRQ
    irq_connect_dynamic(MY_IRQ, 0, my_isr, NULL, 0);

    // Enable the IRQ
    irq_enable(MY_IRQ);

    // Simulate some work
    k_sleep(K_SECONDS(1));

    // Test the interrupt handling
    test_interrupt_handling();

    // Clean up (if necessary)
    irq_disable(MY_IRQ);
}
