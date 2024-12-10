
#include <zephyr/kernel.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/irq.h>
#include <zephyr/init.h>
#include <zephyr/device.h>
#include <zephyr/toolchain.h>

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
    // *(volatile uint32_t *)0xXXXXXX = 0;
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

int main(void)
{
    // Initialize the interrupt
    IRQ_CONNECT(MY_IRQ, 0, my_isr, NULL, 0);
    irq_enable(MY_IRQ);

    // Trigger the interrupt (simulated)
    // This part depends on how you can trigger the interrupt on your hardware
    // For example, if you have a specific register to trigger the interrupt:
    // *(volatile uint32_t *)0xXXXXXX = 1;

    // Test the interrupt handling
    test_interrupt_handling();

    return 0;
}
