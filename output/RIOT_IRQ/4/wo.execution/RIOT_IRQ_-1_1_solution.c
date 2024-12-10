
#include "zephyr/kernel.h"
#include "zephyr/irq.h"
#include "zephyr/sys/atomic.h"

// Define an atomic variable to verify interrupt state
static atomic_t interrupt_state = ATOMIC_INIT(0);

// Interrupt handler function
void my_isr(const void *arg)
{
    // Set the atomic variable to indicate an interrupt occurred
    atomic_set(&interrupt_state, 1);
}

int main(void)
{
    // Register the interrupt handler
    IRQ_CONNECT(DT_IRQN(DT_NODELABEL(my_interrupt_controller)),
                DT_IRQ(DT_NODELABEL(my_interrupt_controller), priority),
                my_isr, NULL, 0);

    // Enable the interrupt
    irq_enable(DT_IRQN(DT_NODELABEL(my_interrupt_controller)));

    // Disable interrupts
    unsigned int key = irq_lock();

    // Check if interrupts are disabled
    if (atomic_get(&interrupt_state) == 0) {
        printk("Interrupts are disabled correctly.\n");
    } else {
        printk("Interrupts are not disabled correctly.\n");
    }

    // Restore interrupts
    irq_unlock(key);

    // Wait for an interrupt to occur
    k_sleep(K_SECONDS(1));

    // Check if an interrupt occurred
    if (atomic_get(&interrupt_state) == 1) {
        printk("Interrupt occurred correctly.\n");
    } else {
        printk("Interrupt did not occur.\n");
    }

    return 0;
}
