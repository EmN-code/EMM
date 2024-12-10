
#include <zephyr/kernel.h>
#include <zephyr/irq.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/sys/printk.h>

#define IRQ_LINE 1  // Example IRQ line, adjust according to your hardware

static atomic_t interrupt_flag = ATOMIC_INIT(0);

void my_isr(const void *arg)
{
    ARG_UNUSED(arg);

    // Set the interrupt flag
    atomic_set(&interrupt_flag, 1);

    // Clear the interrupt (if necessary)
    // This part depends on the specific hardware and interrupt controller
    // For example, if using a specific peripheral, you might need to clear a status register
}

int main(void)
{
    printk("Starting interrupt handling test on ESP32\n");

    // Register the ISR
    IRQ_CONNECT(IRQ_LINE, 0, my_isr, NULL, 0);

    // Enable the interrupt
    irq_enable(IRQ_LINE);

    // Simulate an interrupt by triggering the IRQ line (this depends on your hardware setup)
    // For example, if using a timer or external interrupt, you would configure that here

    // Wait for the interrupt to occur
    while (atomic_get(&interrupt_flag) == 0) {
        k_sleep(K_MSEC(100));
    }

    // Verify that the interrupt was handled
    if (atomic_get(&interrupt_flag) == 1) {
        printk("Interrupt was successfully handled!\n");
    } else {
        printk("Interrupt handling failed!\n");
    }

    // Disable the interrupt
    irq_disable(IRQ_LINE);

    // Verify that interrupts are disabled
    // Trigger the interrupt again (if possible) and check that it is not handled
    // This part depends on your hardware setup

    printk("Interrupt handling test completed\n");

    return 0;
}
