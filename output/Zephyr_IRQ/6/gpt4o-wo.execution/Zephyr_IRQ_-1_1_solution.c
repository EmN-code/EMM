
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

    // Trigger the IRQ (this is a simulation, actual triggering depends on hardware)
    // For example, on ESP32, you might trigger a timer or a specific peripheral IRQ
    // Here, we just simulate the interrupt by setting the flag manually
    atomic_set(&interrupt_flag, 1);

    // Verify that the interrupt flag is set
    if (atomic_get(&interrupt_flag) == 1) {
        printk("Interrupt flag set while interrupts are disabled. Test failed.\n");
    } else {
        printk("Interrupt flag not set while interrupts are disabled. Test passed.\n");
    }

    // Restore interrupts
    irq_unlock(key);

    // Verify that the interrupt flag is set after restoring interrupts
    if (atomic_get(&interrupt_flag) == 1) {
        printk("Interrupt flag set after restoring interrupts. Test passed.\n");
    } else {
        printk("Interrupt flag not set after restoring interrupts. Test failed.\n");
    }

    return 0;
}
