
#include <zephyr/kernel.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/irq.h>

#define MY_IRQ 15  // Example IRQ number for ESP32

static atomic_t interrupt_flag = ATOMIC_INIT(0);

void my_isr(const void *arg)
{
    ARG_UNUSED(arg);

    // Set the interrupt flag
    atomic_set(&interrupt_flag, 1);
}

int main(void)
{
    // Attach the ISR to the IRQ
    irq_connect_dynamic(MY_IRQ, 0, my_isr, NULL, 0);

    // Enable the IRQ
    irq_enable(MY_IRQ);

    // Main loop
    while (1) {
        // Disable interrupts
        unsigned int key = irq_lock();

        // Check if the interrupt flag is set
        if (atomic_get(&interrupt_flag) == 1) {
            printk("Interrupt occurred while interrupts were disabled!\n");
        } else {
            printk("No interrupt occurred while interrupts were disabled.\n");
        }

        // Clear the interrupt flag
        atomic_set(&interrupt_flag, 0);

        // Restore interrupts
        irq_unlock(key);

        // Wait for a while before checking again
        k_sleep(K_MSEC(1000));
    }

    return 0;
}
