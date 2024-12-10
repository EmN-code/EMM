
#include <zephyr/kernel.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/irq.h>
#include <zephyr/init.h>

#define MY_IRQ 14  // Example IRQ number for ESP32

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

    // Simulate some critical section code
    k_sleep(K_MSEC(100));

    // Verify that the interrupt flag is not set
    if (atomic_get(&interrupt_flag) != 0) {
        printk("Error: Interrupt flag set while interrupts were disabled\n");
    } else {
        printk("Interrupts disabled successfully\n");
    }

    // Restore interrupts
    irq_unlock(key);

    // Wait for the interrupt to occur
    k_sleep(K_MSEC(100));

    // Verify that the interrupt flag is set
    if (atomic_get(&interrupt_flag) != 1) {
        printk("Error: Interrupt flag not set after interrupts were restored\n");
    } else {
        printk("Interrupts restored successfully\n");
    }
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
