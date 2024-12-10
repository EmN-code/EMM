
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
    // Here, we simulate the IRQ by manually calling the ISR
    my_isr(NULL);

    // Check if the interrupt flag was set
    if (atomic_get(&interrupt_flag) == 1) {
        printk("Interrupt was handled while interrupts were disabled!\n");
    } else {
        printk("Interrupt was not handled while interrupts were disabled.\n");
    }

    // Restore interrupts
    irq_unlock(key);

    // Clear the interrupt flag
    atomic_set(&interrupt_flag, 0);

    // Trigger the IRQ again
    my_isr(NULL);

    // Check if the interrupt flag was set
    if (atomic_get(&interrupt_flag) == 1) {
        printk("Interrupt was handled after interrupts were restored.\n");
    } else {
        printk("Interrupt was not handled after interrupts were restored.\n");
    }

    return 0;
}
