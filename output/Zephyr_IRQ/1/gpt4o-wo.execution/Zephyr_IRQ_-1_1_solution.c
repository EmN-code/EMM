#include <zephyr/types.h>
#include <zephyr/sys/util.h>
#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zephyr/sys/atomic.h>

// Atomic variable for interrupt check
static atomic_t interrupt_flag = ATOMIC_INIT(0);

// Interrupt handling function
void my_isr_handler(void *arg) {
    ARG_UNUSED(arg);
    // Set the atomic variable
    atomic_set(&interrupt_flag, 1);
    printk("Interrupt handled!\n");
}

void interrupt_setup(void) {
    // Example irq handler setup; pseudo code since exact details need hardware specifics
    // Replace "EXAMPLE_IRQ" and "MY_ISR_VECTOR" with actual values for your MCU
    IRQ_CONNECT(EXAMPLE_IRQ, 0, my_isr_handler, NULL, 0);
    irq_enable(EXAMPLE_IRQ);
}

// Main function
int main(void) {
    printk("Starting interrupt handling example...\n");
    interrupt_setup();

    uint32_t key;
    // Disable interrupts
    key = irq_lock();
    printk("Interrupts disabled, critical section start.\n");

    // Simulate processing
    k_sleep(K_MSEC(100));

    // Restore interrupts
    irq_unlock(key);
    printk("Interrupts enabled, critical section end.\n");

    // Check the atomic variable set by ISR
    if (atomic_cas(&interrupt_flag, 1, 0)) {
        printk("Interrupt was successfully handled and atomic variable reset.\n");
    } else {
        printk("Interrupt was not handled within the time frame.\n");
    }
    
    return 0;
}