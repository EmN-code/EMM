
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
    printk("Starting interrupt handling test on ESP32\n");

    // Attach the ISR to the IRQ
    irq_connect_dynamic(MY_IRQ, 0, my_isr, NULL, 0);

    // Enable the IRQ
    irq_enable(MY_IRQ);

    // Simulate some work
    for (int i = 0; i < 10; i++) {
        // Disable interrupts
        unsigned int key = irq_lock();

        // Check if the interrupt flag was set while interrupts were disabled
        if (atomic_get(&interrupt_flag) == 1) {
            printk("Interrupt occurred while interrupts were disabled\n");
        } else {
            printk("No interrupt occurred while interrupts were disabled\n");
        }

        // Restore interrupts
        irq_unlock(key);

        // Clear the interrupt flag
        atomic_set(&interrupt_flag, 0);

        // Wait for a short period to allow the interrupt to occur
        k_sleep(K_MSEC(100));
    }

    printk("Interrupt handling test completed\n");
    return 0;
}
