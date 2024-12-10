
#include <zephyr/kernel.h>
#include <zephyr/irq.h>
#include <zephyr/sys/atomic.h>

#define MY_IRQ 15  // Example IRQ number for ESP32

static atomic_t interrupt_flag = ATOMIC_INIT(0);

void my_isr(const void *arg)
{
    ARG_UNUSED(arg);

    // Set the interrupt flag
    atomic_set(&interrupt_flag, 1);

    // Clear the interrupt (if necessary)
    // This is hardware-specific and should be handled according to the datasheet
}

int main(void)
{
    // Register the ISR for the specific IRQ
    irq_connect_dynamic(MY_IRQ, 0, my_isr, NULL, 0);

    // Enable the IRQ
    irq_enable(MY_IRQ);

    // Main loop to check the interrupt flag
    while (1) {
        // Disable interrupts
        unsigned int key = irq_lock();

        // Check the interrupt flag
        if (atomic_get(&interrupt_flag) == 1) {
            printk("Interrupt occurred!\n");
            // Clear the interrupt flag
            atomic_set(&interrupt_flag, 0);
        }

        // Restore interrupts
        irq_unlock(key);

        // Sleep for a while to avoid busy-waiting
        k_sleep(K_MSEC(100));
    }

    return 0;
}
