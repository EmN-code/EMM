
#include <zephyr/kernel.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/irq.h>
#include <zephyr/init.h>
#include <zephyr/device.h>

#define MY_IRQ 15  // Example IRQ number, adjust according to your ESP32 configuration

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

    // Trigger the interrupt (this is a simulation, actual triggering depends on hardware)
    // For example, you might use a timer or a specific hardware trigger
    // Here, we just simulate the interrupt by setting the flag manually
    my_isr(NULL);

    // Verify that the interrupt flag is set
    if (atomic_get(&interrupt_flag) == 1) {
        printk("Interrupt flag set correctly while interrupts are disabled.\n");
    } else {
        printk("Interrupt flag not set correctly.\n");
    }

    // Restore interrupts
    irq_unlock(key);

    // Clear the interrupt flag
    atomic_set(&interrupt_flag, 0);
}

int main(void)
{
    // Register the ISR for the specific IRQ
    irq_connect_dynamic(MY_IRQ, 0, my_isr, NULL, 0);

    // Enable the IRQ
    irq_enable(MY_IRQ);

    // Test the interrupt handling
    test_interrupt_handling();

    return 0;
}