#include <zephyr/sys/util.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/kernel.h>

static atomic_t interrupt_flag;

void irq_handler(const void *arg)
{
    ARG_UNUSED(arg);
    atomic_set(&interrupt_flag, 1);
}

int main(void) {
    int key;
    atomic_set(&interrupt_flag, 0);

    printk("Disabling interrupts...\n");
    key = irq_lock();
    if (!atomic_get(&interrupt_flag)) {
        printk("Interrupts are successfully disabled.\n");
    }

    printk("Restoring interrupts...\n");
    irq_unlock(key);

    if (!atomic_get(&interrupt_flag)) {
        printk("Interrupts are successfully restored.\n");
    }

    return 0;
}