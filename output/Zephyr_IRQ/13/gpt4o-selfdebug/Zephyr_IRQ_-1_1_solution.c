#include <zephyr/toolchain.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/types.h>
#include <zephyr/sys/slist.h>
#include <zephyr/sys/mutex.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/arch/xtensa/irq.h>

static atomic_t interrupt_state;

void interrupt_handler(void)
{
    // This function will be invoked by the interrupt
    printk("Interrupt handled!\n");
    atomic_set(&interrupt_state, 1);
}

int main(void)
{
    printk("Starting interrupt handling test!\n");

    atomic_clear(&interrupt_state);

    unsigned int key = irq_lock();

    // Register the interrupt handler here
    // For demonstration purposes, assume direct function calling
    irq_connect_dynamic(0, 0, interrupt_handler, NULL, 0);

    irq_unlock(key);

    if (atomic_get(&interrupt_state) == 0) {
        printk("Interrupts were successfully disabled during critical section.\n");
    } else {
        printk("Unexpected interrupt state during critical section.\n");
    }

    return 0;
}