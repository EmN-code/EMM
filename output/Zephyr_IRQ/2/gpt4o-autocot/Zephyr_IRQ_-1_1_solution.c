
#include <zephyr/kernel.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/irq.h>
#include <zephyr/init.h>
#include <zephyr/device.h>
#include <zephyr/toolchain.h>

#define MY_IRQ 15  // Example IRQ number, adjust according to your ESP32 configuration

static atomic_t interrupt_flag = ATOMIC_INIT(0);

void my_isr(const void *arg)
{
    ARG_UNUSED(arg);

    // Set the interrupt flag
    atomic_set(&interrupt_flag, 1);

    // Acknowledge the interrupt (if needed)
    // This part depends on the specific hardware and driver
}

void configure_interrupt(void)
{
    // Configure the interrupt
    irq_connect_dynamic(MY_IRQ, 0, my_isr, NULL, 0);

    // Enable the interrupt
    irq_enable(MY_IRQ);
}

int main(void)
{
    printk("Starting interrupt handling test on ESP32\n");

    // Configure the interrupt
    configure_interrupt();

    // Main loop to check the interrupt flag
    while (1) {
        // Disable interrupts
        unsigned int key = irq_lock();

        // Check the interrupt flag
        if (atomic_get(&interrupt_flag) == 1) {
            printk("Interrupt was handled correctly\n");
            atomic_set(&interrupt_flag, 0);  // Reset the flag
        }

        // Restore interrupts
        irq_unlock(key);

        // Sleep for a while to avoid busy-waiting
        k_sleep(K_MSEC(100));
    }

    return 0;
}
