
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

    // Acknowledge the interrupt (if necessary)
    // This part depends on the specific hardware and driver
}

void test_interrupt_handling(void)
{
    // Disable interrupts
    unsigned int key = irq_lock();

    // Perform some critical section code
    printk("Critical section: Interrupts are disabled\n");

    // Verify that the interrupt flag is not set
    if (atomic_get(&interrupt_flag) != 0) {
        printk("Error: Interrupt flag set while interrupts are disabled\n");
    } else {
        printk("Interrupt flag is not set as expected\n");
    }

    // Restore interrupts
    irq_unlock(key);

    // Wait for the interrupt to occur
    k_sleep(K_MSEC(100));

    // Verify that the interrupt flag is set
    if (atomic_get(&interrupt_flag) != 1) {
        printk("Error: Interrupt flag not set after interrupt occurred\n");
    } else {
        printk("Interrupt flag set as expected\n");
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
