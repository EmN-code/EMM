#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/init.h>
#include <zephyr/sys/util.h>
#include <zephyr/toolchain.h>
#include <zephyr/sys/mutex.h>

void irq_handler(void *arg) {
    int *interrupt_flag = (int *)arg;
    
    irq_disable(0); // Disable interrupts
    *interrupt_flag = 1; // Set flag
    irq_enable(0); // Re-enable interrupts
}

int main() {
    int atomic_flag = 0;
    
    // Simulated setup of interrupt and handler invocation
    irq_handler(&atomic_flag);
    
    if (atomic_flag) {
        printk("Interrupt handled successfully.\n");
    } else {
        printk("Interrupt handling failed.\n");
    }
    return 0;
}