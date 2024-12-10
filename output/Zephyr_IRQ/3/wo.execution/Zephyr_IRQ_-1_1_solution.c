
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
}

int main(void)
{
    // Attach the ISR to the IRQ
    irq_connect_dynamic(MY_IRQ, 0, my_isr, NULL, 0);

    // Enable the IRQ
    irq_enable(MY_IRQ);

    // Disable interrupts
    unsigned int key = irq_lock();

    // Trigger the interrupt (this is a simplified example, actual triggering depends on hardware)
    // For example, on ESP32, you might trigger an external interrupt or a timer interrupt
    // Here, we just simulate the interrupt by setting a flag
    atomic_set(&interrupt_flag, 0);

    // Restore interrupts
    irq_unlock(key);

    // Wait for the interrupt to be handled
    k_sleep(K_MSEC(100));

    // Check if the interrupt was handled
    if (atomic_get(&interrupt_flag) == 1) {
        printk("Interrupt was handled correctly.\n");
    } else {
        printk("Interrupt was not handled.\n");
    }

    return 0;
}
