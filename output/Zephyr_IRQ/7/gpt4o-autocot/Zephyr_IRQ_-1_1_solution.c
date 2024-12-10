
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
}

void configure_interrupt(void)
{
    // Configure the interrupt
    irq_connect_dynamic(MY_IRQ, 0, my_isr, NULL, 0);
    irq_enable(MY_IRQ);
}

int main(void)
{
    printk("Starting interrupt handling test on ESP32\n");

    // Configure the interrupt
    configure_interrupt();

    // Disable interrupts
    unsigned int key = irq_lock();

    // Trigger the interrupt (this is a placeholder, actual triggering depends on hardware)
    // For example, if using a timer interrupt, you would start the timer here.

    // Wait for the interrupt to be handled
    k_sleep(K_MSEC(100));

    // Check if the interrupt was handled
    if (atomic_get(&interrupt_flag) == 1) {
        printk("Interrupt was handled while interrupts were disabled\n");
    } else {
        printk("Interrupt was not handled while interrupts were disabled\n");
    }

    // Restore interrupts
    irq_unlock(key);

    // Wait for the interrupt to be handled again
    k_sleep(K_MSEC(100));

    // Check if the interrupt was handled
    if (atomic_get(&interrupt_flag) == 1) {
        printk("Interrupt was handled after restoring interrupts\n");
    } else {
        printk("Interrupt was not handled after restoring interrupts\n");
    }

    return 0;
}
