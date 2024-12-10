#include <zephyr/types.h>
#include <zephyr/kernel.h>
#include <zephyr/irq.h>
#include <zephyr/atomic.h>

#define MY_ISR_LINE 4
#define PRIORITY 1

atomic_t interrupt_count;

void my_isr_handler(const void *args) {
    ARG_UNUSED(args);
    atomic_inc(&interrupt_count);
}

void main(void) {
    atomic_clear(&interrupt_count);
    int key = irq_lock();

    irq_connect_dynamic(MY_ISR_LINE, PRIORITY, my_isr_handler, NULL, 0);
    irq_enable(MY_ISR_LINE);

    irq_unlock(key);
    k_sleep(K_SECONDS(1));

    if (atomic_get(&interrupt_count) > 0) {
        printk("Interrupt handling and atomic variable check succeeded\n");
    } else {
        printk("Interrupt handling or atomic variable check failed\n");
    }
}