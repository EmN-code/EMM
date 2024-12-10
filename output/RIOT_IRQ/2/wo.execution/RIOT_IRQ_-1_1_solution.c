
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
    IRQ_CONNECT(DT_IRQN(DT_NODELABEL(my_interrupt_controller)),
                DT_IRQ(DT_NODELABEL(my_interrupt_controller), priority),
                my_isr, NULL, 0);

    // Enable the interrupt
    irq_enable(DT_IRQN(DT_NODELABEL(my_interrupt_controller)));

    // Disable interrupts
    unsigned int key = irq_lock();

    // Check if the interrupt flag is still 0 (no interrupt occurred)
    if (atomic_get(&interrupt_flag) == 0) {
        printk("Interrupts were successfully disabled.\n");
    } else {
        printk("Interrupts were not disabled correctly.\n");
    }

    // Restore interrupts
    irq_unlock(key);

    // Wait for an interrupt to occur
    k_sleep(K_SECONDS(1));

    // Check if the interrupt flag is now 1 (interrupt occurred)
    if (atomic_get(&interrupt_flag) == 1) {
        printk("Interrupt occurred as expected.\n");
    } else {
        printk("Interrupt did not occur.\n");
    }

    return 0;
}
