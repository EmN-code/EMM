#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/init.h>
#include <zephyr/toolchain.h>

volatile atomic_t interrupt_occurred = ATOMIC_INIT(0);

void interrupt_handler(const void *unused)
{
    ARG_UNUSED(unused);
    atomic_set(&interrupt_occurred, 1);
}

int main(void)
{
    /* Define an arbitrary interrupt number, for demonstration purposes */
    unsigned int irq_line = 10;
    
    /* Connect the interrupt handler */
    IRQ_CONNECT(irq_line, 0, interrupt_handler, NULL, 0);

    /* Enable the interrupt line */
    irq_enable(irq_line);

    /* Simulate main loop */
    while (1) {
        if (atomic_get(&interrupt_occurred) == 1) {
            /* Clear the interrupt flag */
            atomic_clear(&interrupt_occurred);
            printk("Interrupt handled\n");
        }
        /* Idle or perform other tasks */
        k_msleep(1000);
    }

    return 0;
}
