#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/arch/arm/aarch32/cortex_m/cmsis.h>

void sample_isr(void)
{
    /* Interrupt Service Routine logic goes here */
    printk("ISR executed\n");
}

int main(void)
{
    /* Atomic variable to track interrupt status */
    atomic_t irq_status = 0;
    uint32_t key;

    printk("Starting interrupt test\n");

    /* Enable IRQs */
    __enable_irq();
    printk("IRQs enabled\n");
    
    /* Simulate critical section, disable IRQs */
    key = irq_lock();
    printk("IRQs disabled\n");
    
    /* Modify atomic variable to simulate work in critical section */
    atomic_set(&irq_status, 1);
    printk("Atomic status set\n");

    /* ... perform operations while IRQs are disabled ... */

    /* Restore IRQs */
    irq_unlock(key);
    printk("IRQs restored\n");

    /* Verify interrupt restoration using atomic status */
    if (atomic_get(&irq_status) == 1) {
        printk("Interrupt handling successful\n");
    } else {
        printk("Interrupt handling failed\n");
    }

    return 0;
}