
#include <zephyr/kernel.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/irq.h>
#include <zephyr/init.h>
#include <zephyr/device.h>
#include <zephyr/toolchain.h>

#define MY_IRQ 15  // Example IRQ number, adjust according to your ESP32 configuration

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

    // Trigger the interrupt (this is a simplified example, actual triggering depends on hardware)
    // For example, you might need to write to a specific register to trigger the interrupt
    // Here we assume the interrupt is triggered by some external event

    // Wait for the interrupt to be handled
    k_sleep(K_MSEC(100));

    // Check if the interrupt flag was set
    if (atomic_get(&interrupt_flag) == 1) {
        printk("Interrupt was handled correctly.\n");
    } else {
        printk("Interrupt was not handled.\n");
    }

    // Restore interrupts
    irq_unlock(key);
}

int main(void)
{
    // Initialize the interrupt
    IRQ_CONNECT(MY_IRQ, 0, my_isr, NULL, 0);
    irq_enable(MY_IRQ);

    // Test the interrupt handling
    test_interrupt_handling();

    return 0;
}
