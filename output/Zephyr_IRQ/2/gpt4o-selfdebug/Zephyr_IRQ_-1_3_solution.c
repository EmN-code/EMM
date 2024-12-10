#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/atomic.h>

#define MY_IRQ_PRIORITY 1  // Define a default priority

volatile atomic_t interrupt_handled = ATOMIC_INIT(0);

void my_isr(void *args) {
    atomic_set(&interrupt_handled, 1);  // Set the atomic variable indicating ISR was handled
}

void enable_my_irq(int irq_line) {
    irq_enable(irq_line);
    irq_set_priority(irq_line, MY_IRQ_PRIORITY);
}

void disable_my_irq(int irq_line) {
    irq_disable(irq_line);
}

int main() {
    // Suppose IRQ line to use is 15 for conceptual purposes
    int irq_line = 15;
    
    enable_my_irq(irq_line);
    
    // Simulate an interrupt firing
    if (atomic_get(&interrupt_handled)) {
        printk("Interrupt handled successfully\n");
        atomic_clear(&interrupt_handled);
    }

    disable_my_irq(irq_line);
    return 0;
}