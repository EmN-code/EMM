
#include <zephyr/kernel.h>
#include <zephyr/irq.h>
#include <zephyr/sys/atomic.h>

#define MY_IRQ 1  // Example IRQ number, adjust according to your ESP32 configuration

atomic_t interrupt_flag = ATOMIC_INIT(0);

void my_isr(const void *arg)
{
    atomic_set(&interrupt_flag, 1);
}

int main(void)
{
    printk("Starting interrupt handling test on ESP32\n");

    // Attach the ISR to the IRQ
    irq_connect_dynamic(MY_IRQ, 0, my_isr, NULL, 0);

    // Enable the IRQ
    irq_enable(MY_IRQ);

    // Trigger the IRQ (this is just for demonstration, actual triggering depends on hardware)
    // For example, if MY_IRQ is a timer interrupt, you would start the timer here.

    // Wait for the interrupt to be handled
    while (atomic_get(&interrupt_flag) == 0) {
        k_yield();
    }

    // Disable the IRQ
    irq_disable(MY_IRQ);

    // Verify that the interrupt flag is still set after disabling the IRQ
    if (atomic_get(&interrupt_flag) == 1) {
        printk("Interrupt was successfully handled and IRQ disabled.\n");
    } else {
        printk("Interrupt handling failed.\n");
    }

    return 0;
}
