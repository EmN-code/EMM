#include <zephyr/toolchain.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
#include <zephyr/irq.h>

#define IRQ_NUM 10  // define IRQ_NUM, ensure to check this number corresponds correctly to your application

volatile atomic_t interrupt_enabled = true;

void my_isr_handler(void *arg) {
    // ISR logic goes here
    atomic_set(&interrupt_enabled, false);
    printk("Interrupt handled and disabled\n");
}

void main(void) {
    printk("Starting interrupt test\n");

    // Connect ISR
    irq_connect_dynamic(IRQ_NUM, 0, my_isr_handler, NULL, 0);
    irq_enable(IRQ_NUM);

    printk("Interrupt connected and enabled\n");

    // Simulating conditions that would trigger the interrupt
    // In a real setup, hardware or specific conditions would trigger this.
    if (atomic_cas(&interrupt_enabled, true, false)) {
        irq_disable(IRQ_NUM);
        printk("Interrupt has been disabled\n");
    }

    printk("Interrupt test completed\n");
}