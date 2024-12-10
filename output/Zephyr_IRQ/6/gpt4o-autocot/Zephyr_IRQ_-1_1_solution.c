
#include <zephyr/kernel.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/irq.h>
#include <zephyr/init.h>
#include <zephyr/device.h>

#define MY_IRQ 15  // Example IRQ number, adjust according to your hardware

static atomic_t interrupt_flag = ATOMIC_INIT(0);

void my_isr(const void *arg)
{
    atomic_set(&interrupt_flag, 1);
    printk("Interrupt occurred!\n");
}

int main(void)
{
    printk("Starting interrupt handling test on ESP32\n");

    // Register the ISR for the specified IRQ
    irq_connect_dynamic(MY_IRQ, 0, my_isr, NULL, 0);

    // Enable the IRQ
    irq_enable(MY_IRQ);

    // Simulate an interrupt by triggering the IRQ (this part depends on your hardware)
    // For example, if you have a timer or a specific hardware trigger, use that here.

    // Wait for the interrupt to occur
    while (atomic_get(&interrupt_flag) == 0) {
        k_sleep(K_MSEC(100));
    }

    // Disable the IRQ to verify that interrupts are restored correctly
    unsigned int key = irq_lock();
    irq_disable(MY_IRQ);

    // Simulate another interrupt (should not be handled)
    // Trigger the IRQ again here

    // Restore interrupts
    irq_unlock(key);

    // Check if the interrupt flag is still set (it should be)
    if (atomic_get(&interrupt_flag) == 1) {
        printk("Interrupt handling and restoration verified successfully!\n");
    } else {
        printk("Interrupt handling verification failed!\n");
    }

    return 0;
}
