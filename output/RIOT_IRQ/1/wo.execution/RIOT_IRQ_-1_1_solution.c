
#include "zephyr/kernel.h"
#include "zephyr/sys/atomic.h"
#include "zephyr/irq.h"

// Define an atomic variable
static atomic_t interrupt_flag = ATOMIC_INIT(0);

// Interrupt handler function
void my_isr(const void *arg)
{
    // Set the atomic variable to 1 to indicate an interrupt occurred
    atomic_set(&interrupt_flag, 1);
}

int main(void)
{
    // Register the interrupt handler
    IRQ_CONNECT(DT_IRQN(DT_NODELABEL(my_interrupt)), DT_IRQ(DT_NODELABEL(my_interrupt)), my_isr, NULL, 0);

    // Enable the interrupt
    irq_enable(DT_IRQN(DT_NODELABEL(my_interrupt)));

    // Loop to check the atomic variable
    while (1) {
        // Disable interrupts
        unsigned int key = irq_lock();

        // Check the atomic variable
        if (atomic_get(&interrupt_flag) == 1) {
            printk("Interrupt occurred and was handled correctly.\n");
            // Reset the atomic variable
            atomic_set(&interrupt_flag, 0);
        }

        // Restore interrupts
        irq_unlock(key);

        // Sleep to avoid busy waiting
        k_sleep(K_SECONDS(1));
    }

    return 0;
}
