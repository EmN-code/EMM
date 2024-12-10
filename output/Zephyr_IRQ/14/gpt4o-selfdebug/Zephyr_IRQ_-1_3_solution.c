#include <zephyr/types.h>
#include <zephyr/toolchain.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/device_mmio.h>
#include <zephyr/syscalls/kernel.h>
#include <zephyr/sys/atomic.h>

#include <zephyr/kernel.h>
#include <zephyr/irq.h>

/* Atomic variable to check interrupt status */
atomic_t interrupt_status = ATOMIC_INIT(0);

/* Interrupt service routine */
void my_interrupt_handler(const void *arg)
{
    atomic_inc(&interrupt_status);
}

int main(void)
{
    /* Define a variable of type irq_lock_key_t to save the state */
    irq_lock_key_t key;

    /* Disable interrupts and save the current state */
    key = irq_lock();

    printk("Interrupts Disabled\n");

    /* Perform critical operations here */

    /* Check atomic variable before restoring interrupts*/
    if (atomic_get(&interrupt_status) == 0) {
        printk("No interrupts occurred during the critical section\n");
    } else {
        printk("Interrupts occurred during the critical section\n");
        atomic_set(&interrupt_status, 0);
    }

    /* Restore interrupts using the saved key */
    irq_unlock(key);

    printk("Interrupts Restored\n");

    return 0;
}