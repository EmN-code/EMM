
#include <zephyr/kernel.h>
#include <zephyr/irq.h>
#include <zephyr/sys/atomic.h>

#define MY_IRQ 15  // Example IRQ number for ESP32

static atomic_t interrupt_flag = ATOMIC_INIT(0);

void my_isr(const void *arg)
{
    ARG_UNUSED(arg);

    // Set the interrupt flag
    atomic_set(&interrupt_flag, 1);
}

int main(void)
{
    // Attach the ISR to the IRQ
    irq_connect_dynamic(MY_IRQ, 0, my_isr, NULL, 0);

    // Enable the IRQ
    irq_enable(MY_IRQ);

    // Disable interrupts
    unsigned int key = irq_lock();

    // Perform some critical section code
    printk("Critical section: Interrupts are disabled\n");

    // Verify that the interrupt flag is not set
    if (atomic_get(&interrupt_flag) == 0) {
        printk("Interrupts were disabled correctly\n");
    } else {
        printk("Interrupts were not disabled correctly\n");
    }

    // Restore interrupts
    irq_unlock(key);

    // Wait for the interrupt to occur
    k_sleep(K_SECONDS(1));

    // Verify that the interrupt flag is set
    if (atomic_get(&interrupt_flag) == 1) {
        printk("Interrupts were restored correctly\n");
    } else {
        printk("Interrupts were not restored correctly\n");
    }

    return 0;
}
