#include zephyr/toolchain.h
#include zephyr/sys/util.h
#include zephyr/sys/slist.h
#include zephyr/sys/mutex.h
#include zephyr/syscalls/device.h

#include <zephyr/kernel.h>
#include <zephyr/irq.h>

volatile atomic_t irq_disabled_counter;
volatile atomic_t irq_enabled_counter;

void irq_handler(const void *irq_param) {
    atomic_inc(&irq_disabled_counter);
    irq_lock();
    printk("Interrupt handled and IRQ locked. Counter: %d\n", irq_disabled_counter);
    irq_unlock(0);
    atomic_inc(&irq_enabled_counter);
    printk("IRQ unlocked. Counter: %d\n", irq_enabled_counter);
}

int main(void) {
    irq_disabled_counter = 0;
    irq_enabled_counter = 0;
    
    int irq_line = 6;  // Hypothetical IRQ line
    
    irq_connect_dynamic(irq_line, 0, irq_handler, NULL, 0);
    irq_enable(irq_line);

    printk("IRQ handler connected and enabled for line %d.\n", irq_line);

    while (1) {
        k_sleep(K_SECONDS(5));
        printk("Current Atomic Counters - Disabled: %d, Enabled: %d\n", irq_disabled_counter, irq_enabled_counter);
    }
    return 0;
}