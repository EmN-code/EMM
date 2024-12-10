
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

    // Verify that the interrupt flag is still 0
    if (atomic_get(&interrupt_flag) == 0) {
        printk("Interrupts were disabled correctly.\n");
    } else {
        printk("Interrupts were not disabled correctly.\n");
    }

    // Restore interrupts
    irq_unlock(key);

    // Wait for the interrupt to occur
    k_sleep(K_MSEC(100));

    // Verify that the interrupt flag is now 1
    if (atomic_get(&interrupt_flag) == 1) {
        printk("Interrupts were restored correctly.\n");
    } else {
        printk("Interrupts were not restored correctly.\n");
    }
}

void main(void)
{
    // Register the ISR for the specified IRQ
    irq_connect_dynamic(MY_IRQ, 0, my_isr, NULL, 0);

    // Enable the IRQ
    irq_enable(MY_IRQ);

    // Test the interrupt handling
    test_interrupt_handling();
}
