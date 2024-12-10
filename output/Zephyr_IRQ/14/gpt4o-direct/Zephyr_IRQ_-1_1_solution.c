
#include <zephyr/kernel.h>
#include <zephyr/irq.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/sys/printk.h>

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

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        // Disable interrupts
        unsigned int key = irq_lock();

        // Perform some critical section work
        printk("Critical section: %d\n", i);

        // Restore interrupts
        irq_unlock(key);

        // Wait for a short period to allow interrupts to occur
        k_sleep(K_MSEC(100));

        // Check if an interrupt occurred
        if (atomic_get(&interrupt_flag) == 1) {
            printk("Interrupt was handled correctly!\n");
            atomic_set(&interrupt_flag, 0);  // Reset the flag
        }
    }

    printk("Interrupt handling test completed\n");
    return 0;
}
