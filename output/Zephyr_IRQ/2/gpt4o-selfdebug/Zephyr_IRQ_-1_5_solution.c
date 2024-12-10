#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/arch/cpu.h>
#include <zephyr.h>

static atomic_t interrupt_count;

void interrupt_handler(const void *arg)
{
    ARG_UNUSED(arg);
    atomic_inc(&interrupt_count);
    printk("Interrupt handled, count: %d\n", atomic_get(&interrupt_count));
}

void trigger_interrupt(void)
{
    unsigned int key = arch_irq_lock();
    /* Simulate an interrupt by calling the handler directly */
    interrupt_handler(NULL);
    arch_irq_unlock(key);
}

int main(void)
{
    printk("Starting interrupt handling test\n");
    atomic_set(&interrupt_count, 0);
    trigger_interrupt();
    k_sleep(K_MSEC(1000));
    printk("Final interrupt count: %d\n", atomic_get(&interrupt_count));
    return 0;
}